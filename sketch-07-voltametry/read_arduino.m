## Copyright (C) 2017 bdobr
## 
## This program is free software; you can redistribute it and/or modify it
## under the terms of the GNU General Public License as published by
## the Free Software Foundation; either version 3 of the License, or
## (at your option) any later version.
## 
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
## 
## You should have received a copy of the GNU General Public License
## along with this program.  If not, see <http://www.gnu.org/licenses/>.

## -*- texinfo -*- 
## @deftypefn {} {@var{retval} =} read_arduino (@var{input1}, @var{input2})
##
## @seealso{}
## @end deftypefn

## Author: bdobr <bdobr@BOGDANEL-DW10>
## Created: 2017-03-17

function retval = read_arduino (port, number, size)
  if (nargin < 3)
    size = 1
  end
 
  retval = zeros (number, size);
  
  pkg load instrument-control
  if (exist ("serial") != 3)
    disp ("No serial support!");
    return
  endif

  try
    s_port = serial (char (["\\\\.\\", port]));
    pause (1);
  catch
    disp ("Invalid serial port!");
    return
  end_try_catch

  set (s_port,  "baudrate", 9600);
  set (s_port,  "bytesize", 8);
  set (s_port,  "parity",   "n");
  set (s_port,  "stopbits", 1);
  set (s_port,  "timeout",  50);

  srl_flush (s_port);
  
  keep_reading = true;
  value = '';
  counter = 1;
  
  while (counter <= number)
    c = srl_read (s_port, 1);
    if (c == 13)
      c = srl_read (s_port, 1);
    endif
    if (c == 10 && value)
        retval(counter,:) = str2num(value);
        counter = counter + 1;
        value = "";
    else
      value = char([value, char(c)]);
    endif
  endwhile

  fclose (s_port);
endfunction