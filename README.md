# NANN — Modern Approximate Nearest Neighbors

The library version is in `CMakeLists.txt` file.

## Introduction

MANN is a library written in the C++ programming language to support both
exact and approximate nearest neighbor searching in spaces of various
dimensions.  Initially, ANN was implemented by David M. Mount of
the University of Maryland, and Sunil Arya of the Hong Kong University of
Science and Technology.  We thank them for open sourcing and maintaining ANN
code.

MANN (Modern ANN) aims at bringing features of the C++14 standard to the
ANN code.  Specifically, macros are replaced by templates and replacing
some data structures by STL containers.  As a result, different Minkowski
norms can be used with the same binary.

This software is based on Approximate Nearest Neighbors Library (ANN), which is
Copyright © 1997–2010 University of Maryland and Sunil Arya and David
Mount. All Rights Reserved.
We inherit ANN licensing, the GNU Lesser General Public License v2.1. See
`CREDITS` and `COPYING` for complete information on terms and conditions of use and
distribution of this software.


## Authors

Paulo R. Urio  
Institute of Mathematical and Computer Sciences  
University of São Paulo  
São Carlos SP, Brazil  

Filipe A. N. Verri  
Institute of Mathematical and Computer Sciences  
University of São Paulo and Arizona State University  
Tempe AZ, USA  

Our e-mail addresses are `surname`@usp.br
