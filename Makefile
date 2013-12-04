
EFFECTS = build/mda.lv2/*.so

ifndef INSTALL_PATH
INSTALL_PATH = ~/.lv2
endif


all:
	python2 ./waf configure --prefix=/tmp/ --lv2dir=${INSTALL_PATH} && python2 ./waf
	for manifest in mda.lv2/manifest-*.in; do \
		sed -e 's/@LIB_EXT@/.so/g' $$manifest > build/$${manifest%.in} ;\
	done
	cp -r mda.lv2/modgui/ build/mda.lv2/

install:
	@for fx in $(EFFECTS); do \
	mkdir -p $(INSTALL_PATH)/mda-`echo $$fx | sed 's/build\/mda.lv2\///' | sed 's/.so//' | tr A-Z a-z`.lv2;\
	cp $$fx $(INSTALL_PATH)/mda-`echo $$fx | sed 's/build\/mda.lv2\///' | sed 's/.so//' | tr A-Z a-z`.lv2;\
	cp `echo $$fx | sed 's/.so/*/'` $(INSTALL_PATH)/mda-`echo $$fx | sed 's/build\/mda.lv2\///' | sed 's/.so//' | tr A-Z a-z`.lv2;\
	cp `echo $$fx | sed 's/.so/.ttl/' | sed 's/mda.lv2\//mda.lv2\/manifest-/'` $(INSTALL_PATH)/mda-`echo $$fx | sed 's/build\/mda.lv2\///' | sed 's/.so//' | tr A-Z a-z`.lv2/manifest.ttl ;\
	mkdir -p $(INSTALL_PATH)/mda-`echo $$fx | sed 's/build\/mda.lv2\///' | sed 's/.so//' | tr A-Z a-z`.lv2/modgui; \
	cp `echo $$fx | sed 's/.so//' | sed 's/mda.lv2\//mda.lv2\/modgui\/*/' | tr A-Z a-z`* $(INSTALL_PATH)/mda-`echo $$fx | sed 's/build\/mda.lv2\///' | sed 's/.so//' | tr A-Z a-z`.lv2/modgui/ || true ;\
	done
	@cd ..

clean:
	./waf clean
