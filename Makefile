all: test_app

test_app: test_app.c
	gcc test_app.c -o test_app