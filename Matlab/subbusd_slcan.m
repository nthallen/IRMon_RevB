classdef subbusd_slcan < handle
  properties
    port
    s
    cur_req_ID
  end
  
  methods
    function self = subbusd_slcan(port)
      if nargin >= 1 && ~isempty(port)
        self.port = port;
      else
        self.port = [];
      end
      s = [];
      self.cur_req_ID = 0;
    end
    
    function serial_port_init(self, port)
      % s = self.serial_port_init(port);
      % [s, port] = self.serial_port_init;
      % Returns an open serial object with timeout set to 0.1
      % The serial object should be closed with the following sequence:
      % fclose(s);
      % delete(s);
      % clear s;
      %
      % If opening the serial port fails, an error dialog is displayed and
      % an empty array is returned.
      if ~isempty(self.s)
        self.serial_port_clear;
      end
      sl = [];
      if (nargin < 2 || isempty(port))
        if ~isempty(self.port)
          port = self.port;
        elseif exist('./CANSerialPort.mat','file')
          rport = load('./CANSerialPort.mat');
          if isfield(rport,'port')
            port = rport.port;
          end
          clear rport
        else
          port = [];
        end
      end
      hw = instrhwinfo('serial');
      if isempty(hw.AvailableSerialPorts)
        port = '';
      elseif length(hw.AvailableSerialPorts) == 1
        port = hw.AvailableSerialPorts{1};
      else
        if ~isempty(port) && ...
            ~any(strcmpi(port, hw.AvailableSerialPorts))
          % This is not a good choice
          port = '';
        end
      end
      if isempty(port)
        if isempty(hw.AvailableSerialPorts)
          % closereq;
          h = errordlg('No serial port found','CAN Serial Port Error','modal');
          uiwait(h);
          return;
        else
          sel = listdlg('ListString',hw.AvailableSerialPorts,...
            'SelectionMode','single','Name','CAN_Port', ...
            'PromptString','Select Serial Port:', ...
            'ListSize',[160 50]);
          if isempty(sel)
            % closereq;
            return;
          else
            port = hw.AvailableSerialPorts{sel};
            save CANSerialPort.mat port
          end
        end
      end
      self.port = port;
      isobj = 0;
      isopen = 0;
      try
        sl = serial(port,'BaudRate',57600,'InputBufferSize',3000);
        isobj = 1;
        fopen(sl);
        isopen = 1;
        set(sl,'Timeout',0.1,'Terminator',13);
        warning('off','MATLAB:serial:fgetl:unsuccessfulRead');
        tline = 'a';
        while ~isempty(tline)
          tline = fgetl(sl);
        end
        fprintf(1, 'Sending initialization:\n');
        fprintf(sl, 'C\nS2\nO\nV\n'); % \n replaced with \r based on Terminator setting
        % A = fread(sl,50);
        tline = 'a';
        while ~isempty(tline)
          tline = fgetl(sl);
          fprintf(1, 'Recd: %s\n', tline);
        end
        self.s = sl;
      catch ME
        h = errordlg(sprintf('Error: %s\nMessage: %s\nport = %s\n', ...
          ME.identifier, ME.message, port), ...
          'CAN Serial Port Error', 'modal');
        uiwait(h);
        if isopen
          fclose(sl);
        end
        if isobj
          delete(sl);
        end
        sl = [];
      end

    end
    
    function serial_port_clear(self)
      % serial_port_clear
      if ~isempty(self.s)
        if strcmp(self.s.Status, 'open')
          fprintf(1,'Closing port %s\n', self.s.Port);
          fclose(self.s);
        end
        delete(self.s);
        self.s = [];
        self.port = [];
      end
      % Clear up any other ports too.
      sl = instrfind;
      if ~isempty(sl)
        for i=1:length(sl)
          if strcmp(sl(i).Status, 'open')
            fprintf(1,'Closing port %s\n', sl(i).Port);
            fclose(sl(i));
          end
          delete(sl(i));
        end
      end
    end
    
    function CAN_send(self, CAN_id, packet)
      pkt = sprintf('t%03X%1X%s', CAN_id, length(packet), sprintf('%02x', packet));
      fprintf(1, 'Sent: %s\n', pkt);
      fprintf(self.s, '%s\r', pkt);
    end
    
    function result = CAN_recv(self, CAN_id)
      % result = CAN_recv(self, CAN_id, cmd_code);
      % CAN_id and cmd_code are the values previously sent to
      % SBCAN_transaction which are used for reference here and
      % error-checking.
      str = fgetl(self.s);
      fprintf(1,'Recd: %s\n', str);
      % check for timeout, error response, etc.
      A = sscanf(str, 't%03X%1X');
      if length(A) ~= 2
        error('Invalid response from CAN: "%s"', str);
      end
      if A(1) ~= CAN_id + 2^6
        error('Invalid response ID: %03X, expected %03X', A(1), CAN_id+2^6);
      end
      B = sscanf(str(6:end), '%02X');
      if A(2) ~= length(B)
        error('dlc (%d) did not match packet length (%d) "%s"', ...
          A(2), length(B));
      end
      result = B;
    end
    
    function ID = next_req_ID(self)
      ID = self.cur_req_ID;
      self.cur_req_ID = self.cur_req_ID + 1;
      if self.cur_req_ID >= 2^6
        self.cur_req_ID = 0;
      end
    end
    
    function response = SBCAN_transaction(self, dev_id, cmd_code, data)
      % response = SBCAN_transaction(dev_id, cmd_code, data);
      % dev_id: the SBCAN device ID
      % cmd_code: the SBCAN command code
      % data: a column vector of data bytes
      % response: a column vector of byte values returned
      % NACK produces and error(), which should be trappable
      % with a try/catch.
      if dev_id < 1 || dev_id > 15
        error('Invalid SBCAN device ID: %.0f', dev_id);
      end
      CAN_ID = dev_id * (2^7) + self.next_req_ID();
      cmd_seq = 0;
      req_len = length(data);
      if (req_len+1) > 32*7
        error('SBCAN transaction data exceeds max length');
      end
      n_packets = ceil((req_len+1)/7);
      bytes_sent = 0;
      req_len_byte = 1;
      req_len_pkt = req_len;
      for cmd_seq = 0:n_packets-1
        CmdSeqByte = cmd_code + cmd_seq*8;
        pktdatalen = min(7-req_len_byte, req_len - bytes_sent);
        pkt = [CmdSeqByte; req_len_pkt; data(bytes_sent+(1:pktdatalen))];
        self.CAN_send(CAN_ID, pkt);
        req_len_byte = 0;
        req_len_pkt = [];
        bytes_sent = bytes_sent + pktdatalen;
      end
      reply = self.CAN_recv(CAN_ID);
      if length(reply) < 2
        error('Short reply (%d)', length(reply));
      end
      if reply(1) ~= cmd_code
        error('Reply cmd_code (%d) expected %d', reply(1), cmd_code);
      end
      reply_len = reply(2);
      response = zeros(reply_len,1);
      nb_pkt_data = length(reply)-2;
      response(1:nb_pkt_data) = reply(3:end);
      nb_recd = nb_pkt_data;
      cmd_seq = 1;
      while nb_recd < reply_len
        reply = self.CAN_recv(CAN_ID);
        if isempty(reply)
          error('Unexpected empty reply');
        end
        if reply(1) ~= cmd_code + cmd_seq*(2^3)
          error('cmd_seq byte (%02X) expected %02X', reply(1), ...
            cmd_code+cmd_seq*(2^3));
        end
        cmd_seq = cmd_seq + 1;
        nb_pkt_data = length(reply)-1;
        response(nb_recd+(1:nb_pkt_data)) = reply(2:end);
        nb_recd = nb_recd + nb_pkt_data;
      end
    end
    
    function values = SBCAN_read_addrs(self, dev_id, addrs)
      % values = SBCAN_read_addrs(dev_id, addrs);
      % dev_id: the SBCAN device ID
      % addrs: a column vector of addresses to read
      % values: a column vector of values read, unsigned
      % NACK produces and error(), which should be trappable
      % with a try/catch.
      if ~iscolumn(addrs)
        if isrow(addrs)
          addrs = addrs';
        else
          error('SBCAN_read_addrs: addrs must be a vector');
        end
      end
      response = self.SBCAN_transaction(dev_id, 0, addrs);
      if length(response) ~= 2*length(addrs)
        warning('SBCAN_read_addrs() response/address mismatch');
      end
      ii = 1:2:length(response);
      values = response(ii) + 256*response(ii+1);
    end
    
    function values = SBCAN_read_incs(self, dev_id, cmd_code, count, addr)
      % values = SBCAN_read_incs(dev_id, cmd_code, count, addr);
      % dev_id: the SBCAN device ID
      % cmd_code: either 1 for read_inc or 2 for read_noinc
      % count: the number of successive registers to read from
      % addr: the starting address
      % values: a column vector of values read, unsigned
      % NACK produces and error(), which should be trappable
      % with a try/catch.
      if ~isscalar(cmd_code) || (cmd_code ~= 1 && cmd_code ~= 2)
        error('SBCAN_read_incs: Invalid cmd_code %d', cmd_code);
      end
      if ~isscalar(count)
        error('SBCAN_read_inc: count must be a scalar');
      end
      if (count < 1 || count > 111)
        error('SBCAN_read_inc: count (%d) out of range [1,111]', count);
      end
      if ~isscalar(addr)
        error('SBCAN_read_inc: addr must be a scalar');
      end
      response = self.SBCAN_transaction(dev_id, cmd_code, [count; addr]);
      if length(response) ~= 2*count
        warning('SBCAN_read_inc() response/count mismatch');
      end
      ii = 1:2:length(response);
      values = response(ii) + 256*response(ii+1);
    end
      
    function values = SBCAN_read_inc(self, dev_id, count, addr)
      % values = SBCAN_read_incs(dev_id, cmd_code, count, addr);
      % dev_id: the SBCAN device ID
      % count: the number of successive registers to read from
      % addr: the starting address
      % values: a column vector of values read, unsigned
      % NACK produces and error(), which should be trappable
      % with a try/catch.
      values = self.SBCAN_read_incs(dev_id, 1, count, addr);
    end
    
    function values = SBCAN_read_noinc(self, dev_id, count, addr)
      % values = SBCAN_read_incs(dev_id, cmd_code, count, addr);
      % dev_id: the SBCAN device ID
      % count: the number of successive registers to read from
      % addr: the starting address
      % values: a column vector of values read, unsigned
      % NACK produces and error(), which should be trappable
      % with a try/catch.
      values = self.SBCAN_read_incs(dev_id, 2, count, addr);
    end
  end
end