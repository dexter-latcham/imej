import sys

if len(sys.argv) > 1:
    first_argument = sys.argv[1]
else:
    exit()

with open(first_argument) as f:
    data = f.readlines()

counter=0
for row in data:
    if row.startswith("#") or row.strip()=="":
        counter+=1
    else:
        break
print("".join(data[:counter]),end="")
print("""#ifdef __cplusplus
extern "C" {
#endif\n""")
print("".join(data[counter:-1]),end="")
print("""
#ifdef __cplusplus
}
#endif
#endif""")
