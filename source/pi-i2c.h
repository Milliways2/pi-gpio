/*
 * I2C.h:
 *	Simplified I2C access routines
 *	Copyright (c) 2013 Gordon Henderson
 ***********************************************************************
 * This file is part of :
 *	https://projects.drogon.net/raspberry-pi//
 *
 *     is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU Lesser General Public License as
 *    published by the Free Software Foundation, either version 3 of the
 *    License, or (at your option) any later version.
 *
 *     is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU Lesser General Public License for more details.
 *
 *    You should have received a copy of the GNU Lesser General Public
 *    License along with .
 *    If not, see <http://www.gnu.org/licenses/>.
 ***********************************************************************
 */

#ifdef __cplusplus
 "C" {
#endif

int i2cOpen(unsigned i2cBus, unsigned i2cAddr) ;
int SMBusOpen(unsigned i2cBus, unsigned i2cAddr);

int i2cRead			(unsigned handle) ;
int i2cRead8		(unsigned handle, unsigned i2cReg) ;
int i2cRead16     (unsigned handle, unsigned i2cReg) ;

int i2cWrite		(unsigned handle, int data) ;
int i2cWrite8    (unsigned handle, unsigned i2cReg, int data) ;
int i2cWrite16    (unsigned handle, unsigned i2cReg, int data) ;

#ifdef __cplusplus
}
#endif
