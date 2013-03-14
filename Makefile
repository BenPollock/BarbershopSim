all: main test

test: fifo.c helpers.c cashierdelay_test.c fifo_test.c haircut_test.c timestampsec2str_test.c timestampstr2sec_test.c servicetimestr2sec_test.c
	gcc -D_BSD_SOURCE -D_XOPEN_SOURCE -g -std=c99 fifo.c fifo_test.c -o fifo_test -lm
	gcc -D_BSD_SOURCE -D_XOPEN_SOURCE -g -std=c99 helpers.c cashierdelay_test.c -o cashierdelay_test -lm -lpthread
	gcc -D_BSD_SOURCE -D_XOPEN_SOURCE -g -std=c99 helpers.c servicetimestr2sec_test.c -o servicetimestr2sec_test -lm -lpthread
	gcc -D_BSD_SOURCE -D_XOPEN_SOURCE -g -std=c99 helpers.c haircut_test.c -o haircut_test -lm -lpthread
	gcc -D_BSD_SOURCE -D_XOPEN_SOURCE -g -std=c99 helpers.c timestampsec2str_test.c -o timestampsec2str_test -lm -lpthread
	gcc -D_BSD_SOURCE -D_XOPEN_SOURCE -g -std=c99 helpers.c timestampstr2sec_test.c -o timestampstr2sec_test -lm -lpthread
main: helpers.c barbershop.c customer.c fifo.c
	gcc -D_BSD_SOURCE -D_XOPEN_SOURCE -g -std=c99 helpers.c barbershop.c customer.c fifo.c -o barbershop -lm -lpthread -lrt
