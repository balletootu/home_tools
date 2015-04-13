
month = {'Jan': '01', 'Feb': '02','Mar': '03','Apr': '04','May': '05','Jun': '06','Jul': '07','Aug': '08','Sep': '09','Oct': '10','Nov': '11','Dec': '12'}    

f_in = open('2007.txt', 'r')
names = f_in.readlines()
f_in.close()

f_out = open('rename.txt', 'w')

rename_list = []
for i in names:
#    print(i)
    rename_str = i.strip()
    sp_str1 = rename_str.split(' - ')
#    print(sp_str1)
    if len(sp_str1) > 3:
        sp_str2 = sp_str1[0].split(' ')
        data_str = '{0}-{1}-{2}'.format(sp_str2[2], month[sp_str2[1]], sp_str2[0])
        rename_str = '{0} - {1} - {2} (x{3})'.format(data_str, sp_str1[-1], sp_str1[1], int(sp_str1[2].split(' ')[0]))
        rename_list.append(rename_str)
        print(rename_str)
    
for i in rename_list:
    f_out.write(i + '\n');
f_out.close()