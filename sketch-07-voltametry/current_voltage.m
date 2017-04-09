## Author: bdobr <bdobr@BOGDANEL-DW10>
## Created: 2017-04-09

function retval = current_voltage (D)
  retval = [ 0, 0 ];
  m = size (D, 1);
  for c = 2:m;
    if D(c, 1) <= D(c-1, 1)
      continue;
    endif
    voltage = D(c, 2);
    current = (D(c-1, 2) - D(c, 2)) / (D(c, 1) - D(c-1, 1)) ;
    retval = [ retval ; [ current, voltage ] ];
  endfor
endfunction