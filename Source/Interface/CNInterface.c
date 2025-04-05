/*
 * @file CNInterface.h
 * @description Define interface of this framework
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#import <BasicKit/CNInterface.h>
#include <stdarg.h>
#include <stdio.h>

static struct CNInterfaceProtocol * s_interface ;

static int s_printf(const char * format, ...) ;

void
CNInitInterface(struct CNInterfaceProtocol * prot)
{
        if(prot == NULL){
                static struct CNInterfaceProtocol s_default_interface ;
                s_interface = &s_default_interface ;

                s_interface->printf = s_printf ;
                s_interface->error  = s_printf ;
        } else {
                s_interface = prot ;
        }
}

const struct CNInterfaceProtocol *
CNInterface(void)
{
        return s_interface ;
}

static int
s_printf(const char * format, ...)
{
        va_list arg_ptr;
        va_start(arg_ptr, format);
        int result = vprintf(format, arg_ptr);
        va_end(arg_ptr);
        return result ;
}
