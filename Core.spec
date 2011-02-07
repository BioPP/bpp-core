%define name bpp-core
%define version 2.0.0
%define release 1
%define _prefix /usr

Summary: The Bio++ Core library.
Name: %{name}
Version: %{version}
Release: %{release}
Vendor: The Bio++ Project
Source: http://kimura.univ-montp2.fr/BioPP/Repositories/sources/%{name}-%{version}.tar.gz
License: CeCILL 2
Group: System Environment/Libraries
BuildRoot: %{_builddir}/%{name}-root
Packager: Julien Dutheil
AutoReq: yes
AutoProv: yes

%description
This library contains the core classes and utilitary functions of the Bio++ project.

%package devel
Summary: Libraries, includes to develop applications with %{name}.
Group: Development/Libraries
Requires: %{name} = %{version}

%description devel
The %{name}-devel package contains the header files and static libraries for
building applications which use %{name}.

%prep
%setup -q

%build
CFLAGS="$RPM_OPT_FLAGS" cmake -DCMAKE_INSTALL_PREFIX=%{_prefix}
make

%install
rm -rf $RPM_BUILD_ROOT
make DESTDIR=$RPM_BUILD_ROOT install

%clean
rm -rf $RPM_BUILD_ROOT

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%files
%defattr(-,root,root)
%doc AUTHORS COPYING INSTALL ChangeLog
%{_prefix}/lib/lib*.so
%{_prefix}/lib/lib*.so.*

%files devel
%defattr(-,root,root)
%doc AUTHORS COPYING INSTALL ChangeLog
%{_prefix}/lib/lib*.a
%{_prefix}/include/*

%changelog
* Mon Feb 07 2011 Julien Dutheil <julien.dutheil@univ-montp2.fr>
- Version 2.0.0

