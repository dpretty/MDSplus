Name: mdsplus-globus
Version: 1.2
Release: 1
Copyright: GNU GPL
Group: Applications/Acquisition
Source: mdsplus-1.2.tar.gz
Prefix: /usr/local
Buildroot: /var/tmp/mdsplus
Vendor: MIT Plasma Science and Fusion Center 
Summary: MDSplus Data Acquisition System

AutoReqProv: no

%ifos Linux
requires: openmotif
%endif

%description
Main libraries and routines to get MDS operational

%prep
%setup

%build
./configure --exec_prefix=$RPM_BUILD_ROOT/usr/local/mdsplus --enable-nodebug --enable-globus
make

%install
rm -rf $RPM_BUILD_ROOT
make install

%clean
rm -rf $RPM_BUILD_ROOT

%post 
$RPM_INSTALL_PREFIX/mdsplus/rpm/post_install_script

%postun

MDSPLUS_DIR=`cat /etc/.mdsplus_dir`
$MDSPLUS_DIR/local/mdsplus_post_uninstall_script

%files
%defattr(-,root,root)
/usr/local/mdsplus/LabView
/usr/local/mdsplus/bin
/usr/local/mdsplus/etc
/usr/local/mdsplus/idl
/usr/local/mdsplus/include
/usr/local/mdsplus/java
/usr/local/mdsplus/lib
/usr/local/mdsplus/man
/usr/local/mdsplus/rpm
/usr/local/mdsplus/setup.csh
/usr/local/mdsplus/setup.sh
/usr/local/mdsplus/tdi
/usr/local/mdsplus/trees
/usr/local/mdsplus/uid
%dir /usr/local/mdsplus/local/tdi

%changelog
* Wed Aug 29 2000 Basil P. DUVAL <basil.duval@epfl.ch>
- version 1.0
