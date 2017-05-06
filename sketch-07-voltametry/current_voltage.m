## Author: bdobr <bdobr@BOGDANEL-DW10>
## Created: 2017-04-09

function retval = current_voltage (D)
  retval = [ 0, 0 ];
  m = size (D, 1);
  for c = 2:m;
    if D(c, 1) <= D(c-1, 1)
      continue;
    endif
    % the voltage is read directly from ADC
    voltage = D(c, 2);
    % the current is estimated from I = dQ/dt = d(CU)/dt = CdU/dt
    current = (D(c-1, 2) - D(c, 2)) / (D(c, 1) - D(c-1, 1)); 
    
    %voltage = (5/1024) * voltage;  % V
    %current = (5/10240) * current; % mA, 100nF capacitor, time is in ms
    
    retval = [ retval ; [ current, voltage ] ];
  endfor
endfunction