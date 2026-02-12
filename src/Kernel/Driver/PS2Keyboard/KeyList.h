#pragma once
#include "Ps2KeyboardDriver.h"




/*
    idk how else to do this
    so we just map everything like this
    currently based on the finnish keyboard layout
    please dont change

    -sigmacat
*/

static struct Key keymap_FI[] = {
    {29, "§", "½"},   // empty

    {0x2, "1", "!"},   
    {0x3, "2", "\""},   
    {0x4, "3", "#"},   
    {0x5, "4", "$"},   
    {0x6, "5", "%"},   
    {0x7, "6", "&"},   
    {0x8, "7", "/"}, 
    {0x9, "8", "("}, 
    {0xa, "9", ")"}, 
    {0xb, "0", "="}, 

    {0xc, "+", "?"}, 
    {0xd, "-", "_"}, 
    //{0xe, " ", " "},

    {0x10, "q", "Q"},
    {0x11, "w", "W"},
    {0x12, "e", "E"},
    {0x13, "r", "R"},
    {0x14, "t", "T"},
    {0x15, "y", "Y"},
    {0x16, "u", "U"},
    {0x17, "i", "I"},
    {0x18, "o", "O"},
    {0x19, "p", "P"},
    {0x1a, "[", "{"},
    {0x1b, "]", "}"},

    {0x1e, "a", "A"},
    {0x1f, "s", "S"},
    {0x20, "d", "D"},
    {0x21, "f", "F"},
    {0x22, "g", "G"},
    {0x23, "h", "H"},
    {0x24, "j", "J"},
    {0x25, "k", "K"},
    {0x26, "l", "L"},
    {0x27, ";", ":"},
    {0x28, "'", "*"},
//    {0x2b, "\\", "|"},
    {0x56, "<", ">"},
    {0x2c, "z", "Z"},
    {0x2d, "x", "X"},
    {0x2e, "c", "C"},
    {0x2f, "v", "V"},
    {0x30, "b", "B"},
    {0x31, "n", "N"},
    {0x32, "m", "M"},
    {0x33, ",", "<"},
    {0x34, ".", ">"},
    {0x35, "-", "_"},



    // "special" keys

    {0x39, " ", " "}, //space
    {0x1c, "!ENTER!", "!ENTER!"}, //ENTER
    {0xe, "!BACK!", "!BACK!"}, //Back space
    






    /*
    enter = 0x1c
    shift = 0x2a

    right shift = 0x36

    ctrl = 0x1d
    
    windows = 0x5d
    alt = 0x38
    space = 0x39


    arrow UP = 0x48
    arrow DOWN = 0x50
    arrow RIGHT = 0x4d
    arrow LEFT = 0x4b
    */
};
