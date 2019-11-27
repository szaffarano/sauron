# Sauron

Small, simple and useless compiler that produces x386 binaries based on a proprietary language. Made in 2001 during my engineering bachelor's degree, forming a team with Mariano Testasecca (such a great team!). The name was chosen because of J.R.R. Tolkien's book "Lord of the Rings".

## Dependencies

```sh
$ sudo apt-get install gcc-multilib byacc build-essential
```

## Build

```sh
# ignore some warnings!
# to be fixed ;)
$ make
```

## Usage

```sh
$ cat samples/ej04.sau
main {
   b = 1 + 4;
   a = 5;
   if ( a < 6 ) {
#     a = 6;    #
      if ( b < 6 )
        a = 1;
#      else
        a = 2;    #
   }
   else {
      a = 4;
   }
   print a;
   print b;
} endmain

$ ./parser samples/ej04.sau -o test04

$ ./test04
1
5
```
