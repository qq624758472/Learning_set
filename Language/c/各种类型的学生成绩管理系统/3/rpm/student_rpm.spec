%define pkgrelease  2
%if 0%{?test}
%define specrelease %{pkgrelease}
%else
## allow specrelease to have configurable %%{?dist} tag in other distribution
%define specrelease %{pkgrelease}%{?dist}
%endif

%bcond_with check
%global _unpackaged_files_terminate_build 0
%global debug_package   %{nil}
%global common_description %{expand:
This is test dynamic-link library, which is a test interface dynamic-link library to adapt to kms}

# Arches on which we need to prevent.
%define multilib_arches %{ix86} ia64 %{mips} x86_64

Name:          student_rpm
Version:       1.0.0
Release:       %{specrelease}
Summary:       student_rpm test

License:       secrut
URL:           null
Source0:        %{name}-%{version}.tar.gz

BuildRequires: gcc make
BuildRequires: glibc-devel

%description
test.

%package libs
Summary: A test dynamic-link library
Requires:glibc

%description libs
test is a dynamic-library for test.

%package devel
Summary: Files for development of applications which will use test
Requires: %{name}-libs%{?_isa}
Requires: pkgconfig

%description devel
Summary: A test dynamic-link library
Requires: %{name}-libs%{?_isa} = %{epoch}:%{version}-%{release}
Requires: pkgconfig

%prep
%setup

%build
make


%install
# Install test.
install -d $RPM_BUILD_ROOT{%{_bindir},%{_includedir},%{_includedir}/license,%{_libdir},/usr/lib/license,%{_mandir},%{_libdir}/license,%{_pkgdocdir}}
#make DESTDIR=$RPM_BUILD_ROOT install
#rename so so.%{version} $RPM_BUILD_ROOT%{_libdir}/license/*.so
#for lib in $RPM_BUILD_ROOT%{_libdir}/*.so ; do
#        chmod 755 ${lib}
#        ln -s -f `basename ${lib}` $RPM_BUILD_ROOT%{_libdir}/`basename ${lib} .%{version}`
#done

install -m644 student $RPM_BUILD_ROOT/usr/bin/student
install -m644 server.h $RPM_BUILD_ROOT%{_includedir}/

LD_LIBRARY_PATH=`pwd`${LD_LIBRARY_PATH:+:${LD_LIBRARY_PATH}}
export LD_LIBRARY_PATH

%post -p /sbin/ldconfig
%postun -p /sbin/ldconfig

%files libs
%{!?_licensedir:%global license %%doc}
#%license LICENSE
%dir /usr/bin/
%attr(0755,root,root) /usr/bin/student

%files devel
#%doc CHANGES doc/dir-locals.example.el doc/student.el
%{_prefix}/include/

%changelog
* Wed Jan 13 2021 test <test@test.com> - 1.0.0
- student init