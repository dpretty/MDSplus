import subprocess,datetime,os,sys
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
    p=subprocess.Popen('%s/mdsplus/devscripts/makeDebian %s %s %s %d %s' % (WORKSPACE,FLAVOR,pkg,VERSION,release,DIST),shell=True,cwd=os.getcwd())
    return p.wait()

def makeDebsCommand(args):
    DIST=getLsbReleaseDist()
    WORKSPACE=getWorkspace()
    FLAVOR=getFlavor()
    for d in ['debian','SOURCES','DEBS','BUILDROOT','EGGS']:
        try:
            os.mkdir("%s%s%s" % (WORKSPACE,os.sep,d))
        except:
            pass
    VERSION=getVersion()
    HW,BITS=getHardwarePlatform()
    if FLAVOR=="stable":
        debflavor=""
        pythonflavor=""
    else:
        debflavor="-"+FLAVOR
        pythonflavor=FLAVOR+"-"
    need_to_build=False
    updates=dict()
    for pkg in getPackages():
        updates[pkg]=dict()
        updates[pkg]['Update']=False
        RELEASE_TAG=getReleaseTag(pkg)
        updates[pkg]['Release']=getRelease(pkg)
        if RELEASE_TAG is None:
            print "No releases yet for %s mdsplus-%s. Building." % (FLAVOR,pkg)
            updates[pkg]['Update']=True
        else:
            c=checkRelease(pkg)
            if len(c) > 0:
                updates[pkg]['Update']=True
                updates[pkg]['Release']=updates[pkg]['Release']+1
                print "New %s release for mdsplus-%s. Building.\n==========================" % (FLAVOR,pkg)
                for line in c:
                    print line
                print "================================="
            else:
                debfile="%s/DEBS/%s/mdsplus%s-%s-%s-%d.%s.%s.deb" % (WORKSPACE,HW,debflavor,pkg,VERSION,updates[pkg]['Release'],DIST,HW)
                try:
                    os.stat(debfile)
                except Exception,e:
                    print "%s missing. Rebuilding." % (debfile,)
                    updates[pkg]['Update']=True
        if updates[pkg]['Update']:
            need_to_build=True
    status="ok"
    if need_to_build:
        p=subprocess.Popen('tar zcf %s/SOURCES/mdsplus%s-%s.tar.gz --exclude CVS ../mdsplus;' % (WORKSPACE,debflavor,VERSION) +\
                    './configure --enable-mdsip_connections --enable-nodebug --exec_prefix=%s/BUILDROOT/usr/local/mdsplus --with-gsi=/usr:gcc%d;' % (WORKSPACE,BITS) +\
                    'make;make install',shell=True,cwd=os.getcwd())
        build_status=p.wait()
        print "%s, Done building - status=%d" % (str(datetime.datetime.now()),build_status)
        if build_status != 0:
            print "Error building mdsplus. Status=%d" % (build_status,)
            status="error"
        else:
            for pkg in getPackages():
                if updates[pkg]['Update']:
                    debfile="%s/DEBS/%s/mdsplus%s-%s-%s-%d.%s.%s.deb" % (WORKSPACE,HW,debflavor,pkg,VERSION,updates[pkg]['Release'],DIST,HW)
                    build_status=createDeb(WORKSPACE,FLAVOR,pkg,VERSION,updates[pkg]['Release'],DIST)
                    if build_status != 0:
                        print "Error building debian package %s, status=%d" % (debfile,build_status)
                        sys.exit(build_status)
                    writeRpmInfo("%s/DEBS/%s/mdsplus%s-%s-%s-%d.%s.%s" % (WORKSPACE,HW,debflavor,pkg,VERSION,updates[pkg]['Release'],DIST,HW))
        if updates['python']['Update']:
            p=subprocess.Popen('env MDSPLUS_PYTHON_VERSION="%s%s-%s" python setup.py bdist_egg' % (pythonflavor,VERSION,updates['python']['Release']),shell=True,cwd="%s/mdsplus/mdsobjects/python"%(WORKSPACE))
            python_status=p.wait()
            if python_status != 0:
                print "Error building MDSplus-%s%s-%s" % (pythonflavor,VERSION,updates['python']['Release'])
            else:
                p=subprocess.Popen('mv dist/* %s/EGGS/;rm -Rf dist'%(WORKSPACE,),shell=True,cwd="%s/mdsplus/mdsobjects/python"%(WORKSPACE))
                p.wait()
    else:
        print 'All DEBS are up to date'
        status="skip"
    if status=="ok":
        print "Build completed successfully. Checking for new releaseas and tagging the modules"
        for pkg in getPackages():
            print "Checking %s for new release" % (pkg,)
            if updates[pkg]['Update']:
                print "      New release. Tag modules with %s %s %s %s" % (FLAVOR,VERSION,updates[pkg]['Release'],DIST)
                newRelease(pkg,FLAVOR,VERSION,updates[pkg]['Release'],DIST)
            else:
                print "      No changes, skipping"
    if status=="error":
        sys.exit(1)
    p=subprocess.Popen('rsync -av DEBS ../%s/;rsync -av SOURCES ../%s/;rsync -av EGGS ../%s/' % (FLAVOR,FLAVOR,FLAVOR),shell=True,cwd=WORKSPACE)
    sys.exit(p.wait())
#!/bin/bash
