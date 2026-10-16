// OptimizeAnalyzers.java
// Pre-script to disable whole-program decompiler parameter ID analyzer to speed up analysis

import ghidra.app.script.GhidraScript;

public class OptimizeAnalyzers extends GhidraScript {
    @Override
    public void run() throws Exception {
        println("[*] Optimizing analyzers for targeted analysis on " + currentProgram.getName());
        setAnalysisOption(currentProgram, "Decompiler Parameter ID", "false");
    }
}
