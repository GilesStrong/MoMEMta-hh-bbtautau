from __future__ import division
import os
import os.path
import optparse

dataPrefix = "/eos/user/g/gstrong/CMS_HH_bbtautau_MVA/Data/"
installPrefix = "/afs/cern.ch/user/g/gstrong/momemta/MoMEMta-hh-bbtautau/build/"
setup = "/afs/cern.ch/user/g/gstrong/momemta/setup.sh"
pwd = os.getcwd()

def makeJOFile(start, stop, inputFile, queue):
    name = inputFile.split("/")[-1].split(".")[0]
    inputFile = dataPrefix + inputFile
    joName = name + "_" + str(start) + ".job"
    joFile = open(joName, "w")
    joFile.write("echo Beginning\ job\n")
    joFile.write("source " + setup + "\n")
    joFile.write("echo Paths\ set\n")
    joFile.write("cd " + installPrefix + "\n")
    joFile.write("./ttbar_mu_tau.exe -i " + inputFile + " -s " + str(start) + " -e " + str(stop))
    joFile.close()
    sub = "bsub -q " + queue + " " + joName
    print "Submitting: " + sub
    os.system("chmod 744 " + joName)
    os.system(sub)

if __name__ == "__main__":
    parser = optparse.OptionParser(usage = __doc__)
    parser.add_option("-i", "--input", dest = "input", action = "append", default = ["mu_tau_b_b_MCData"], help = "Input file")
    parser.add_option("-n", "--nEvents", dest = "nEvents", action = "append", default = [202252], help = "Number of events")
    parser.add_option("-a", "--first", dest = "first", action = "append", default = [0], help = "First event numbers")
    parser.add_option("-s", "--split", dest = "split", action = "append", default = [250], help = "Number of events per split")
    parser.add_option("-q", "--queue", dest = "queue", action = "append", default = ["1nd"], help = "Queue to run jobs")

    opts, args = parser.parse_args()
    nEvents = int(opts.nEvents[-1])
    first = int(opts.first[-1])
    split = int(opts.split[-1])
    inputFile = opts.input[-1]
    queue = opts.queue[-1]
    runNumbers = range(first, first+nEvents, split)

    print "Setting " + str(len(runNumbers)) + " jobs to run on queue " + queue + " over " + str(nEvents/len(runNumbers)) + " events each,"
    print "between event numbers " + str(first) + " and " + str(first+nEvents) + " of file " + inputFile
    if raw_input("Continue? [y/n]: ").lower().strip() == "y":
        for i in runNumbers:
            makeJOFile(i, i+split, inputFile, queue)
