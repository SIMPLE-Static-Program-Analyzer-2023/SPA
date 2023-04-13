from bs4 import BeautifulSoup
import sys

with open('out.xml', 'r') as f:
    data = f.read() 
file = open("parsedOut.txt", "w")
bs_data = BeautifulSoup(data, 'xml') 
failed = bs_data.find_all("failed")
check = True
for failure in failed:
    check=False
    failed_id = failure.find_previous_siblings("id")
    for id in failed_id:
        failed_id_no = id.get_text()
        file.write(failed_id_no + '\n')
    query = failure.find_previous_siblings("querystr")
    for q in query:
        qu = q.get_text()
        file.write(qu + '\n')

file.flush()
file.close()
if not check:
    sys.exit(1)
else:
    sys.exit(0)
    