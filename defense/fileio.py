from defense.canclass import CANMessage


# Returns .log file contents as [timestamp, [message id, message data]]
def log_parser(log_file_name):
    if log_file_name.find('.log') == -1:
        return []

    in_file = open(log_file_name)
    raw_data = [line.split(' ') for line in in_file]
    # timestamp, network_name, msg_id#msg_data
    in_file.close()

    msgs = []
    for packet in raw_data:
        temp = [packet[0], packet[2].split('#')]
        msgs.append(temp)

    can_msg_objs = []
    for line in msgs:
        ts_field = float(line[0][1:-1])  # Delete beginning and ending parentheses
        id_field = line[1][0]
        data_field = '0x' + line[1][1][:-1]  # Delete newline at end
        data_field = format(int(data_field, 16), '064b')  # 8 bytes of data
        can_msg_objs.append(CANMessage(ts_field, id_field, data_field))

    return can_msg_objs


# Writes to .csv as timestamp, message id, message data
def write_to_csv(csv_file_name, can_data):
    if len(can_data) < 1:
        return
    if isinstance(can_data[0], CANMessage) is False:
        return
    if csv_file_name.find('.csv') == -1:
        return

    out_file = open(csv_file_name, 'w')
    for datum in can_data:
        out_file.write('%s%s%s%s%s\n' % (datum.timestamp, ',',
                                         datum.id, ',', datum.data))
    out_file.close()
