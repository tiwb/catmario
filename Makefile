debug:
	emcc -g4 -s NO_EXIT_RUNTIME=1 dxlib.cpp loadg.cpp main.cpp -o dlm.html --preload-file res

release:
	emcc -O3 -s NO_EXIT_RUNTIME=1 -s OUTLINING_LIMIT=2000 dxlib.cpp loadg.cpp main.cpp -o dlm.html --preload-file res
