all:
	cd bin2c; make;
	cd c2bin; make;
	cd testapp; make;

clean:
	cd bin2c; make clean;
	cd c2bin; make clean;
	cd testapp; make clean;

test: all
	cd tests; ./testscript.sh
