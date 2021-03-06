#include "MyHeader.h"
#include "OneGraph.h"

int main(int argc, char * argv[]) {
	// 1. The variables to store the input and output file names or folders.
	string sInputFileNameNodes; // Input file name for nodes of the network.
	string sInputFileNameEdgesPhysical; // Input file name for edges of the physical network.
	string sInputFileNameEdgesConceptual; // Input file name for edges of the conceptual network.
	string sInputFileNameSeeds; // Input file name for the seed nodes.
	string sOutputFolder; // The folder to output the results.
	double dGammaThreshold = 1.5; // 0 - 2 . the degree threshold in the greedy node deletion algorithm.
	bool   bSizeConstraint = false; // do we need to find a subgraph with size constraint.
	int    nNumOfNodesInSubgraph = 40; // the size of the resulting subgraph.

	// 2. Parsing the input parameters.
	int nIdx = 1; // index for parsing the input parameters.
	while (nIdx < argc)
	{
		string sToken1 = argv[nIdx];
		string sToken2 = argv[nIdx + 1];
		nIdx = nIdx + 2;
		if (sToken1.compare("-n") == 0){ // Input file name for nodes of the dual networks
			sInputFileNameNodes = sToken2;
		} else if (sToken1.compare("-p") == 0){ // Input file name for edges of the physical network
			sInputFileNameEdgesPhysical = sToken2;
		} else if (sToken1.compare("-c") == 0){ // Input file name for edges of the conceptual network
			sInputFileNameEdgesConceptual = sToken2;
		} else if (sToken1.compare("-s") == 0){ // Input file name for seed nodes
			sInputFileNameSeeds = sToken2;
		} else if (sToken1.compare("-o") == 0){ // Output file path
			sOutputFolder = sToken2;
		} else if (sToken1.compare("-g") == 0){ // parameter \gamma
			dGammaThreshold = atof(sToken2.c_str());
		} else if (sToken1.compare("-k") == 0){ // k for DCS_k (the DCS problem with size constraint)
			bSizeConstraint = true;
			nNumOfNodesInSubgraph = atoi(sToken2.c_str());
		}
	}

	// toy dataset in Fig 4
/*	sInputFileNameNodes = "../ToyDualGraphs/01Nodes.txt"; // GeneName SingleLocusTestValue
	sInputFileNameEdgesPhysical = "../ToyDualGraphs/02EdgesP.txt"; // nIndexOfGene1 nIndexOfGene2 dTwoLocusTestValue
	sInputFileNameEdgesConceptual = "../ToyDualGraphs/03EdgesC.txt"; // nIndexOfGene1 nIndexOfGene2 dTransProb dEdgeWeight
	sInputFileNameSeeds = "../ToyDualGraphs/04Seeds.txt";
	sOutputFolder  = "../ToyDualGraphs/Output/";
	bSizeConstraint = true;
	nNumOfNodesInSubgraph = 4; */

	// 3. The main variable: an instance of the COneGraph class.
	COneGraph aOneGraph;

	// 4. Set the input parameters and read the input files.
	aOneGraph.SetOutputFolder(sOutputFolder);
	aOneGraph.ReadNodesFile(sInputFileNameNodes);
	aOneGraph.ReadEdgesFilePhysical(sInputFileNameEdgesPhysical);
	aOneGraph.ReadEdgesFileConceptual(sInputFileNameEdgesConceptual);
	aOneGraph.ReadSeedNodeFile(sInputFileNameSeeds);
	aOneGraph.SetDegreeThresholdInNodeDeletion(dGammaThreshold);
	if (bSizeConstraint == true) {
		aOneGraph.SetDCSKSizeConstraint(nNumOfNodesInSubgraph);
	}

	// 5. Pre-processing the graph.
	aOneGraph.SetUpAdjacentNodesAndIncidentEdgesForEachNode();
	aOneGraph.CalculateNodeDegreeInConceptualNetwork();

	// 6. The DCS_GND algorithm begins.
	aOneGraph.GreedyFindSubgraphConnectInPPIAndDenseInGGI();
	aOneGraph.GetTheDensestSubgraph();
	aOneGraph.OutputTheDensestSubgraph();
	aOneGraph.OutputSummaryOfDCS();

	return 0;
}
