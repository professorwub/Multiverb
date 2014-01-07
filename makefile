BUNDLE = Mverb.lv2
INSTALL_DIR = /usr/local/lib/lv2


$(BUNDLE): manifest.ttl Mverb.ttl Mverb.so
	rm -rf $(BUNDLE)
	mkdir $(BUNDLE)
	cp manifest.ttl Mverb.ttl Mverb.so $(BUNDLE)

Mverb.so: Mverb.cpp Multiverb.cpp DelayLine.cpp Multiverb.h DelayLine.h
	g++ -shared -fPIC -DPIC Mverb.cpp Multiverb.cpp DelayLine.cpp `pkg-config --cflags --libs lv2-plugin` -o Mverb.so

install: $(BUNDLE)
	mkdir -p $(INSTALL_DIR)
	rm -rf $(INSTALL_DIR)/$(BUNDLE)
	cp -R $(BUNDLE) $(INSTALL_DIR)

clean:
	rm -rf $(BUNDLE) Mverb.so
