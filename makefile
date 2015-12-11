OUTDIR="/usr/bin"

build: ./src/qserver.c ./src/qclient.c
	gcc ./src/qserver.c -o pushover_server -lrt
	gcc ./src/qclient.c -o pushover_client -lrt
install: build
	mkdir -p ${OUTDIR}
	mv pushover_server ${OUTDIR}/.
	mv pushover_client ${OUTDIR}/.
	chmod +x ${OUTDIR}/pushover_server
	chmod +x ${OUTDIR}/pushover_client
clean:
	rm -f pushover_server
	rm -f pushover_client
	rm -f ${OUTDIR}/pushover_server
	rm -f ${OUTDIR}/pushover_client
test: ./src/test.c
	gcc ./src/test.c -o test -lrt
	#sh test
