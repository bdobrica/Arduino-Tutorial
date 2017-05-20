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
## @deftypefn {} {@var{retval} =} linear_regression (@var{input1}, @var{input2})
##
## @seealso{}
## @end deftypefn

## Author: bdobr <bdobr@BOGDANEL-DW10>
## Created: 2017-03-18

function theta = linear_regression (X, y)
  hold on
  plot (X, y, "b+");
  m = size (X, 1);
  minX = min(X);
  maxX = max(X);
  intv = (minX:0.5:maxX)';
  X = [ones(m,1), X];
  intv = [ones(size(intv,1),1), intv];
  theta = pinv(X' * X) * X' * y;
  vals = intv * theta;
  plot (intv(:,2), vals, "r-");
  hold off
endfunction
