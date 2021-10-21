class aborted_tasks:
    def __init__(self, index, arrival, proc_time, period, abs_deadline, finished, observed_proc_time):
        self.index = index
        self.arrival = arrival
        self.proc_time = proc_time
        self.period = period
        self.abs_deadline = abs_deadline
        self.finished = finished
        self.observed_proc_time = observed_proc_time

class finished_tasks:
    def __init__(self, index, arrival, proc_time, period, abs_deadline, finished, observed_proc_time):
        self.index = index
        self.arrival = arrival
        self.proc_time = proc_time
        self.period = period
        self.abs_deadline = abs_deadline
        self.finished = finished
        self.observed_proc_time = observed_proc_time


#read trace data into a list of objects
def readIntoList(program, a_list, f_list):
    tokens = []
    for line in program:
        tokens = line.split(":")
        if tokens[0] == "a":
            a_list.append(finished_tasks(tokens[1], tokens[2], tokens[3], tokens[4], tokens[5], tokens[6], tokens[7]))
        if tokens[0] == "f":
            f_list.append(finished_tasks(tokens[1], tokens[2], tokens[3], tokens[4], tokens[5], tokens[6], tokens[7]))


#calculate avg processing time in a program
def calcAvgProcTime(list, name):
    total_proc_time = 0
    for i in list:
        total_proc_time += int(i.proc_time)
    print("Avg proc time for " + name + ":\t" + str("%.2f" % round(total_proc_time / len(list), 2)))

#calculate avg response time in a program
def calcAvgResponseTime(list, name):
    total_response_time = 0
    for i in list:
        curr_response_time = int(i.finished) - int(i.arrival)
        total_response_time += curr_response_time
    print("Avg resp time for " + name + ":\t" + str("%.2f" % round(total_response_time / len(list), 2)))

#calculate avg lateness if late and earliness if early
def calcAvgLateness(list, name):
    total_lateness = 0
    no_of_late_0 = 0
    no_of_late_1 = 0
    no_of_late_2 = 0
    total_earliness = 0
    for i in list:
        if(int(i.finished) - int(i.abs_deadline)) > 0:
            if(int(i.index) == 0):
                no_of_late_0 += 1
            if(int(i.index) == 1):
                no_of_late_1 += 1
            if(int(i.index) == 2):
                no_of_late_2 += 1
            curr_lateness = int(i.finished) - int(i.abs_deadline)
            total_lateness += curr_lateness
        elif(int(i.finished) - int(i.abs_deadline)) < 0:
            curr_earliness = int(i.abs_deadline) - int(i.finished)
            total_earliness += curr_earliness
    print("Avg  lateness  for " + name + ":\t" + str("%.2f" % round(total_lateness / len(list), 2)))
    print(f"Task 0 was late {no_of_late_0} times for {name}");
    print(f"Task 1 was late {no_of_late_1} times for {name}");
    print(f"Task 2 was late {no_of_late_2} times for {name}");
    print("Avg earliness for " + name + ":\t" + str("%.2f" % round(total_earliness / len(list), 2)))
    print("\n")


def checkDiff(list, name):
    diff0 = 0
    diff1 = 0
    diff2 = 0

    for i in list:
        if int(i.proc_time) != int(i.observed_proc_time):
            if(int(i.index) == 0):
                diff0 += abs(int(i.proc_time) - int(i.observed_proc_time))
            if(int(i.index) == 1):
                diff1 += abs(int(i.proc_time) - int(i.observed_proc_time))
            if(int(i.index) == 2):
                diff2 += abs(int(i.proc_time) - int(i.observed_proc_time))
    print(f"Total diff for task 0 is {diff0} for {name}")
    print(f"Total diff for task 1 is {diff1} for {name}")
    print(f"Total diff for task 2 is {diff2} for {name}")
    print("\n")


prog1_edf = open("trace-prog1-edf.data", "r")
prog1_rm = open("trace-prog1-rm.data", "r")
prog2_edf = open("trace-prog2-edf.data", "r")
prog2_rm = open("trace-prog2-rm.data", "r")  

aborted_list_prog1_edf = []
aborted_list_prog1_rm = []
aborted_list_prog2_edf = []
aborted_list_prog2_rm = []

finished_list_prog1_edf = []
finished_list_prog1_rm = []
finished_list_prog2_edf = []
finished_list_prog2_rm = []

readIntoList(prog1_edf, aborted_list_prog1_edf, finished_list_prog1_edf)
readIntoList(prog1_rm, aborted_list_prog1_rm, finished_list_prog1_rm)
readIntoList(prog2_edf, aborted_list_prog2_edf, finished_list_prog2_edf)
readIntoList(prog2_rm, aborted_list_prog2_rm, finished_list_prog2_rm)

print("")
print("Processing time: ")
calcAvgProcTime(finished_list_prog1_edf, "edf 1")
calcAvgProcTime(finished_list_prog1_rm, " rm 1")
calcAvgProcTime(finished_list_prog2_edf, "edf 2")
calcAvgProcTime(finished_list_prog2_rm, " rm 2")

print("")
print("Response time: ")
calcAvgResponseTime(finished_list_prog1_edf, "edf 1")
calcAvgResponseTime(finished_list_prog1_rm, " rm 1")
calcAvgResponseTime(finished_list_prog2_edf, "edf 2")
calcAvgResponseTime(finished_list_prog2_rm, " rm 2")

print("")
print("Lateness")
calcAvgLateness(finished_list_prog1_edf, "edf 1")
calcAvgLateness(finished_list_prog1_rm, " rm 1")
calcAvgLateness(finished_list_prog2_edf, "edf 2")
calcAvgLateness(finished_list_prog2_rm, " rm 2")

print("")
print("Obs_proc time proc_time diff")
checkDiff(finished_list_prog1_edf, "edf 1")
checkDiff(finished_list_prog1_rm, "rm 1")
checkDiff(finished_list_prog2_edf, "edf 2")
checkDiff(finished_list_prog2_rm, "rm 2")










    










