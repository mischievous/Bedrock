//
//  dataBuffer.hpp
//  spartacus
//
//  Created by Alexandra Beebe on 2/4/22.
//

#ifndef DATABUFFER
#define DATABUFFER

//
#include "stdint.h"

typedef unsigned char uint08_t;

//
class dataBuffer
{
    private:
        // generic buffer data
        uint64_t       _lngth_;
        uint64_t       _size_;

        //
        union
        {
            uint64_t       actual;
            uint08_t      *buffer;
        };

    public :
        static  size_t callback_load      ( void *data, size_t size, size_t nmemb, dataBuffer *self )
        {
            self->grow ( nmemb );

            //
            memcpy (self->offset(), data, nmemb);
            self->append ( nmemb );

//            printf ("callback_load : %zu [ %llu ] : %f\n", nmemb, self->_lngth_, ((double) self->_lngth_) / ((double) self->_size_));

            //
            return nmemb;
        }

        //
                      dataBuffer ( uint64_t size = 4096, uint64_t debug = 0 );
        virtual      ~dataBuffer ( void );


        //
        void          secure     ( const char *path );
        void          access     ( const char *path );

        // operators
//        uint08_t     *operator() ( void ) { return buffer; }
//        const char   *operator() ( void ) { return (char *) buffer; }
        operator const char *( void  ) { return (char *) buffer; };
        operator       void *( void  ) { return (void *) buffer; };

//        void         *operator() ( void ) { return (void *) buffer; }

        char         *operator+= ( char     *append );
        uint08_t     *operator+= ( uint08_t *append ) { return (uint08_t *) this->operator+= ( (char *) append); }


        //
        uint64_t      length     ( void ) { return _lngth_;}
        uint64_t      size       ( void ) { return _size_ ;}

        //
        void          grow       ( uint64_t size   ) { reserve (size + _lngth_); }

        //
        uint64_t      fixed      ( uint64_t length ) { _lngth_  = length; return length; };
        uint64_t      append     ( uint64_t length ) { _lngth_ += length; return length; }
        void          append     ( const char *format, ... );


        void          reserve    ( uint64_t size);
        void          clear      ( uint64_t clearMemory = 0 );

        //
        void          *offset    ( uint64_t value ) { if (value > _lngth_) return NULL; return (void *) &buffer [ value ]; }
        void          *offset    ( void           ) {return (void *) &buffer [ _lngth_ ];}

        uint64_t       offset    ( uint64_t value, uint64_t offset )
        {
            if (value < offset)
                return 0;

            if ((value < actual) || (value > (actual + _lngth_)))
                return 0;

            return ( value - actual ) - offset;
        }


//        // Sizes
//        unsigned int  getSize   (void)                 {return size;}
//        unsigned int  setSize   (unsigned int size);
//
//        // Lengths
//        unsigned int  getLength (void)                 {return bytes;}
//        void          setLength (unsigned int length);
//        void          rstLength (unsigned int length);
//
//        //
//        void          reset     (unsigned int length = 0);
//
//        //
//        void          *offset   (unsigned int off)     {return (void *) &memory[off];}
//        void          *offset   (void)                 {return (void *) &memory[bytes];}
//        void          *data     (void)                 {return (void *) memory;}
//
//
//
//        unsigned int  append    (unsigned int len, void *src);
//        unsigned int  append    (char src);
//        bool          setByte   (unsigned int off, char  byte);
//        char          getByte   (unsigned int off);
//
//        void          dump      (char *msg);
//        void          setMirror (monitorBuffer *iMirror) {mirror = iMirror;}
//
//        void          byteSwap  (char byte);


};

#endif /* dataBuffer_hpp */
