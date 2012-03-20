import subprocess,datetime,os,sys,shutil
from pkg_utils import getLsbReleaseDist, getWorkspace, getFlavor, getVersion, getRelease, getReleaseTag, checkRelease, getPackages, makeSrcTar, newRelease, getHardwarePlatform
from pkg_rpms import writeRpmInfo
def writeDebInfo(outfile):
    f=open(outfile+'-info.html','w')
    url="http://hudson.mdsplus.org/job/%s/%s" % (os.environ['JOB_NAME'],os.environ['BUILD_NUMBER'])
    f.write('<html>\n<head>\n'+
            '<meta http-equiv="Refresh" content="0; url=%s" />\n' % (url,) + 
            '</head>\n<body>\n<p>For more info please follow <a href="%s">this link</a>.</p>\n' % (url,)+
            '</body>\n</html>\n')
    f.close()

def createDeb(WORKSPACE,FLAVOR,pkg,VERSION,release,DIST):
    p=subprocess.Popen('%s/devscripts/makeDebian %s %s %s %d %s' % (WORKSPACE,FLAVOR,pkg,VERSION,release,DIST),shell=True,cwd=os.getcwd())
    return p.wait()

def prepareRepo(repodir):
    for f in ('conf','pool','dists','db'):
        try:
            shutil.rmtree(repodir+'/'+f)
        except:
            pass
        os.mkdir(repodir+'/'+f)
    f=open(repodir+'/conf/distributions','w')
    f.write(
"""Origin: MDSplus Development Team
Label: MDSplus
Codename: MDSplus
Version: 1.0
Architectures: i386 amd64 source
Components: alpha beta stable
Description: MDSplus packages
SignWith: MDSplus
""")
    f.close()

def getDebfile(WORKSPACE,arch,debflavor,pkg,VERSION,updates):
    return "%s/DEBS/%s/mdsplus%s-%s-%s.%d_%s.deb" % (WORKSPACE,arch,debflavor,pkg,VERSION,updates[pkg]['Release'],arch)
    
def makeDebsCommand(args):
    DIST=getLsbReleaseDist()
    WORKSPACE=getWorkspace()
    FLAVOR=getFlavor()
    DISTPATH=args[2]+'/'+DIST+'/'+FLAVOR+'/'
    need_to_build=len(args) > 3
    for d in ['debian','SOURCES','DEBS','BUILDROOT','EGGS','REPO']:
        try:
            os.mkdir("%s%s%s" % (WORKSPACE,os.sep,d))
        except:
            pass
    prepareRepo("%s/REPO" % (WORKSPACE,))
    VERSION=getVersion()
    HW,BITS=getHardwarePlatform()
    arch={32:'i386',64:'amd64'}[BITS]
    if FLAVOR=="stable":
        debflavor=""
        pythonflavor=""
    else:
        debflavor="-"+FLAVOR
        pythonflavor=FLAVOR+"-"
    updates=dict()
    for pkg in getPackages():
        updates[pkg]=dict()
        updates[pkg]['Update']=False
        updates[pkg]['Tag']=False
        RELEASE_TAG=getReleaseTag(pkg)
        updates[pkg]['Release']=getRelease(pkg)
        if RELEASE_TAG is None:
            print "No releases yet for %s mdsplus-%s. Building." % (FLAVOR,pkg)
            updates[pkg]['Update']=True
            updates[pkg]['Tag']=True
        else:
            c=checkRelease(pkg)
            if len(c) > 0:
                updates[pkg]['Update']=True
		updates[pkg]['Tag']=True
                updates[pkg]['Release']=updates[pkg]['Release']+1
                print "New %s release for mdsplus-%s. Building.\n==========================" % (FLAVOR,pkg)
                for line in c:
                    print line
                print "================================="
            else:
                debfile=getDebfile(WORKSPACE,arch,debflavor,pkg,VERSION,updates)
                try:
                    os.stat(debfile)
                except Exception,e:
                    print "%s missing. Rebuilding." % (debfile,)
                    updates[pkg]['Update']=True
        if updates[pkg]['Update']:
            need_to_build=True
    status="ok"
    if need_to_build:
        cmd='rm -Rf DEBS/* SOURCES/*;' +\
             'ln -sf $(pwd) ../mdsplus%s-%s;' % (debflavor,VERSION) +\
             'tar zcfh SOURCES/mdsplus%s-%s.tar.gz --exclude CVS ' % (debflavor,VERSION) +\
                 '--exclude SOURCES --exclude DEBS --exclude EGGS ../mdsplus%s-%s;' % (debflavor,VERSION) +\
             'rm -f ../mdsplus%s-%s;' % (debflavor,VERSION) +\
             './configure --enable-mdsip_connections --enable-nodebug --exec_prefix=%s/BUILDROOT/usr/local/mdsplus --with-gsi=/usr:gcc%d;' % (WORKSPACE,BITS) +\
             'make;make install;' +\
             'olddir=$(pwd);' +\
             'cd mdsobjects/python;' +\
             'export MDSPLUS_PYTHON_VERSION="%s%s-%s";' % (pythonflavor,VERSION,updates['python']['Release']) +\
             'rm -Rf dist;' +\
             'python setup.py bdist_egg;' +\
             'rsync -a dist %s/BUILDROOT/usr/local/mdsplus/mdsobjects/python/;' % (WORKSPACE,) +\
             'cd $olddir'
        p=subprocess.Popen(cmd,shell=True,cwd=os.getcwd())
        build_status=p.wait()
        sys.stdout.flush()
        print "%s, Done building - status=%d" % (str(datetime.datetime.now()),build_status)
        if build_status != 0:
            print "Error building mdsplus. Status=%d" % (build_status,)
            status="error"
        else:
            build_status=createDeb(WORKSPACE,FLAVOR,'all','1.0',0,DIST)
            if build_status != 0:
		print "Error build catch all package, status=%d" % (build_status,)
                sys.exit(build_status)
            for pkg in getPackages():
                debfile=getDebfile(WORKSPACE,arch,debflavor,pkg,VERSION,updates)
                build_status=createDeb(WORKSPACE,FLAVOR,pkg,VERSION,updates[pkg]['Release'],DIST)
                if build_status != 0:
                    print "Error building debian package %s, status=%d" % (debfile,build_status)
                    sys.exit(build_status)
                writeRpmInfo(debfile[0:-3])
        if updates['python']['Update']:
            sys.stdout.flush()     
            p=subprocess.Popen('env MDSPLUS_PYTHON_VERSION="%s%s-%s" python setup.py bdist_egg' % (pythonflavor,VERSION,updates['python']['Release']),shell=True,cwd="%s/mdsobjects/python"%(WORKSPACE))
            python_status=p.wait()
            if python_status != 0:
                print "Error building MDSplus-%s%s-%s" % (pythonflavor,VERSION,updates['python']['Release'])
            else:
                sys.stdout.flush()
                p=subprocess.Popen('mv dist/* %s/EGGS/;rm -Rf dist'%(WORKSPACE,),shell=True,cwd="%s/mdsobjects/python"%(WORKSPACE))
                p.wait()
    else:
        print 'All DEBS are up to date'
        status="skip"
    if status=="ok":
        sys.stdout.flush()
        sys.path.insert(0,WORKSPACE+'/tests')
        from distribution_tests import test
        test(WORKSPACE,FLAVOR)
        print "Build completed successfully. Checking for new releaseas and tagging the modules"
        p=subprocess.Popen('rsync -av DEBS %s;rsync -av SOURCES %s;rsync -av EGGS %s' % (DISTPATH,DISTPATH,DISTPATH),shell=True,cwd=WORKSPACE)
        pstat=p.wait()
        if pstat != 0:
            print "Error copying files to destination"
            sys.exit(1)
        for d in ('EGGS','REPO'):
            try:
                shutil.rmtree(WORKSPACE+'/'+d)
            except:
                pass
        for pkg in getPackages():
            if updates[pkg]['Tag']:
                print "New release. Tag %s modules with %s %s %s %s" % (pkg,FLAVOR,VERSION,updates[pkg]['Release'],DIST)
                sys.stdout.flush()
                newRelease(pkg,FLAVOR,VERSION,updates[pkg]['Release'],DIST)
    if status=="error":
        sys.exit(1)
