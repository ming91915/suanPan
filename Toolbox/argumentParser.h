/**
 * @fn argumentParser
 * @brief An shapeFunctionQuad function.
 *
 * @author T
 * @date 21/07/2017
 * @file argumentParser.h
 */

#ifndef ARGUMENTPARSER_H
#define ARGUMENTPARSER_H

#include <Step/Bead.h>

//! A simple parser.
void argumentParser(int, char**);

void print_header();
void print_version();
void print_helper();

void cli_mode(const shared_ptr<Bead>&);

#endif
