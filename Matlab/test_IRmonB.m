%%
% Test IRMon_RevB
%%
ssl = subbusd_slcan;
%%
ssl.serial_port_clear;
ssl.serial_port_init;
%%
res = ssl.SBCAN_read_addrs(1,2); % board_ID
if res ~= 12
  fprintf(1, 'Expected Board ID 12 for IRmon_RevB, received %d\n', res);
end
%%
res = ssl.SBCAN_read_addrs(1, [2,3,4]); % response should fit in one pkt
%%
res = ssl.SBCAN_read_addrs(1, [2,3,4,5]); % response requires 2 pkts
%%
% Request requires 2 pkts, response requires 3
res = ssl.SBCAN_read_addrs(1, [2,3,4,5,32,33,34,35]); % request req
%%
res = ssl.SBCAN_read_inc(1, 4, 2);
%%
Nregs = ssl.SBCAN_read_addrs(1, 8); % words in FIFO
vals = ssl.SBCAN_read_noinc(1, Nregs, 9);
