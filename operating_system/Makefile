SUBDIRS= linux-0.12 application driver elf

#命令包的用法。
define make_subdir
	#for 循环的用法。
	for subdir in $(SUBDIRS) ; \
	do \
		( cd $$subdir && make $1) \
	done;
endef

define target
	(cd $(DIR) && make)
endef

all:
	$(call make_subdir )
install :
	$(call make_subdir , install)
debug:
	$(call make_subdir , debug)
clean:
	$(call make_subdir , clean) #后边是参数，传到命令包的参数。

target:
	$(call target)
	