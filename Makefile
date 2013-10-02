all:
	#@echo "Please execute ./test.sh"
	./test.sh
clean:
	rm -f hello.elf

distclean: clean
	rm -rf qemu/build
