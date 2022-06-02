CC=gcc
prom = student
deps = 
obj = main.o server.o
obj_cli = client.o
arg += -pthread -g

.PHONY: rpm clean

$(prom): $(obj)
	$(CC) -o $(prom) $(obj) $(arg)
	-cp student ../
	-cp server.h ../

client:$(obj_cli)
	$(CC) -o client $(obj_cli)

$(obj_cli):%.o:%.c
	$(CC) -c $(arg) $< -o $@

#%.o:%.c 所有的.o目标都依赖于与它同名的.c文件
#当然还有deps中列出的头文件
#$<  第一个依赖文件    $@目标文件   $^所有的依赖文件
.c.o:
	$(CC) -Wall -o $@ -c $< $(arg)


install:
	mkdir -p ${DESTDIR}/usr/bin/
	install -Dm644 ./student  ${DESTDIR}/usr/bin/
	mkdir -p ${DESTDIR}/usr/include/
	install -Dm644 ./server.h ${DESTDIR}/usr/include/server.h

rpm:
	-mkdir -p ~/rpmbuild
	-mkdir -p ~/rpmbuild/BUILD  ~/rpmbuild/BUILDROOT  ~/rpmbuild/RPMS  ~/rpmbuild/SOURCES  ~/rpmbuild/SPECS  ~/rpmbuild/SRPMS
	-mkdir rpm/student_rpm-1.0.0
	-cp ./* ./rpm/student_rpm-1.0.0/ -rf
	-cd ./rpm/ && tar czvf student_rpm-1.0.0.tar.gz ./student_rpm-1.0.0/ && cp ./student_rpm-1.0.0.tar.gz ~/rpmbuild/SOURCES/
	-cd ./rpm/ && rpmbuild -bb student_rpm.spec
	-cp ~/rpmbuild/RPMS/${ARCH}/*.rpm ./rpm/

clean:
	rm -rf $(prom) *.o client
