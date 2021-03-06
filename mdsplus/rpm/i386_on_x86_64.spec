Name: mdsplus
Version: %{?mdsver}
Release: %{?mdsrel}%{?dist}
License: GNU GPL
Group: Applications/Acquisition
Prefix: /usr/local
Source: %{?kitname}.tar.gz
BuildRoot: /var/tmp/mdsplus
Vendor: http://www.mdsplus.org
Summary: MDSplus Data Acquisition System

AutoReqProv: yes

%description
Main libraries and programs to get MDSplus operational

%prep
%setup

%build
cd mdsplus
./configure --exec_prefix=$RPM_BUILD_ROOT/usr/local/mdsplus --enable-nodebug --target=i686-linux
make clean
make
cd ..

%install
cd mdsplus
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
/usr/local/mdsplus/bin
/usr/local/mdsplus/ChangeLog
/usr/local/mdsplus/etc
/usr/local/mdsplus/idl
/usr/local/mdsplus/include
/usr/local/mdsplus/java
/usr/local/mdsplus/LabView
/usr/local/mdsplus/lib
/usr/local/mdsplus/man
/usr/local/mdsplus/mdsobjects
/usr/local/mdsplus/rpm
/usr/local/mdsplus/setup.csh
/usr/local/mdsplus/setup.sh
/usr/local/mdsplus/tdi
/usr/local/mdsplus/trees
/usr/local/mdsplus/uid32
%dir /usr/local/mdsplus/local/tdi

%changelog
* Wed Aug 29 2000 Basil P. DUVAL <basil.duval@epfl.ch>
- version 1.0
