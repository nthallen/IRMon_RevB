%%
% Test IRMon_RevB
%%
sbsl = subbusd_slcan;
%%
sbsl.close;
sbsl.open;
sbsl.verbose_sbslcan = true;
%%
res = sbsl.SBCAN_read_addrs(1,2); % board_ID
if res ~= 12
  fprintf(1, 'Expected Board ID 12 for IRmon_RevB, received %d\n', res);
end
%%
res = sbsl.SBCAN_read_addrs(1, [2,3,4]); % response should fit in one pkt
%%
res = sbsl.SBCAN_read_addrs(1, [2,3,4,5]); % response requires 2 pkts
%%
% Request requires 2 pkts, response requires 3
res = sbsl.SBCAN_read_addrs(1, [2,3,4,5,32,33,34,35]); % request req
%%
res = sbsl.SBCAN_read_inc(1, 4, 2);
%%
Nregs = sbsl.SBCAN_read_addrs(1, 8); % words in FIFO
vals = sbsl.SBCAN_read_noinc(1, Nregs, 9);
%%
vals = sbsl.SBCAN_read_cnt_noinc_bytes(1, 8, 30, 9);
%%
str = sbsl.SBCAN_read_cnt_noinc_char(1, 8, 30, 9);
fprintf(1,'Description = "%s"\n', str);
%%
vals = sbsl.SBCAN_read_cnt_noinc(1, 8, 30, 9);
%%
vals = sbsl.SBCAN_read_inc(1, 6, 64);
%%
try
  sbsl.SBCAN_write_inc(1, 67, 1);
catch ME
  fprintf(1, '%s\n', ME.message);
end
%%
[success, msg] = sbsl.write_ack(1, 67, 1);
if success
  fprintf(1, 'Write succeeded: %s\n', msg);
else
  fprintf(1, 'Write failed: %s\n', msg);
end
%%
if ~sbsl.write_ack(1,67,1)
  fprintf(1, 'Write failed\n');
end
%%
sbsl.close
