OPTIONS = -o dlm.js \
	-s NO_EXIT_RUNTIME=1 \
	--preload-file res \
	--js-library lib.js

SRC = dxlib.cpp loadg.cpp main.cpp

debug:
	emcc -g4 $(OPTIONS) $(SRC)

release:
	emcc -O3 -s OUTLINING_LIMIT=2000 --memory-init-file 0 $(OPTIONS) $(SRC)

clean:
	rm dlm.js* dlm.data