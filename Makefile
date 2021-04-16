# all: pfind
# 	./pfind -p 5 -c dir1 char stR kkk

pfind : pfind.c
	gcc pfind.c -o pfind

clean:
	rm -f pfind ./tasks ./results