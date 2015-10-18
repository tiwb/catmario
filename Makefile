debug:
	emcc -g4 dxlib.cpp loadg.cpp main.cpp -o dlm.html --preload-file res

release:
	emcc -o2 dxlib.cpp loadg.cpp main.cpp -o dlm.html --preload-file res
