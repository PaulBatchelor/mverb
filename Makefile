plugin.so: plugin.cpp
	g++ -O3 -g $< -fPIC -shared -o $@ -lsporth -lsoundpipe -lsndfile -lm

clean:
	rm -rf plugin.so
