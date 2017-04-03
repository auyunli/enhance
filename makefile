kernel_modules = ./src/modules
kernel = ./src/kernel
test_kernel = ./test/kernel
test_algo = ./test/algo

sub_dir =  ./src/*

build_gl:
	$(MAKE) -C ./src/gl all

build_render: build_gl
	$(MAKE) -C ./src/render all

build_math:
	$(MAKE) -C ./src/math all

build_file:
	$(MAKE) -C ./src/file all

build_fileformat:
	$(MAKE) -C ./src/fileformat all

build_datatransform: build_file
	$(MAKE) -C ./src/datatransform all

build_ui:
	$(MAKE) -C ./src/ui all

build_kernel_modules: build_render build_math build_datatransform build_gl build_ui build_fileformat
	$(MAKE) -C $(kernel_modules) all

build_kernel: build_kernel_modules
	$(MAKE) -C $(kernel) all

build_test_algo:
	$(MAKE) -C $(test_algo) all

build_test_kernel: build_kernel
	$(MAKE) -C $(test_kernel) all

.PHONY: all
all: build_test_kernel

.PHONY: clean
clean:
	for dir in $(sub_dir); do \
	  ($(MAKE) -C $$dir clean);\
	done
