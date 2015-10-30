OPTIONS = -o dlm.js \
	-s NO_EXIT_RUNTIME=1 \
	--js-library lib.js

SRC = loadg.cpp main.cpp

debug:
	emcc -g4 $(OPTIONS) $(SRC)

release:
	emcc -O3 -s OUTLINING_LIMIT=2000 --memory-init-file 0 $(OPTIONS) $(SRC)

clean:
	rm dlm.js* dlm.data