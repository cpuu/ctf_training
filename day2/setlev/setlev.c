#include <stdio.h>
#include <string.h>

void parseAndSet( char* prog, char* opt )
{
    char levBuf[8];

    if ( strcmp( opt, "help" ) == 0 ) {
        printf( "%s [opts] --level=N\n", prog );
        return;
    }

    if ( strncmp( opt, "level=", 6 ) == 0 ) {
        strcpy( levBuf, opt+6 );
        printf( "setting privilege level %s\n", levBuf );
        return;
    }
}

int main(int argc, char* argv[])
{
    if ( argc < 2 ) return -1;

    if ( strncmp( argv[1], "--", 2 ) == 0 ) parseAndSet( argv[0], argv[1] + 2 );

    return 0;
}
