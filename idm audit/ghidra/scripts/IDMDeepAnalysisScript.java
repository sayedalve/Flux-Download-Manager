// IDMDeepAnalysisScript.java
// Standalone Ghidra script to trace video detection, IPC, and download panel logic in IDM binaries.

import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.mem.Memory;
import ghidra.program.model.mem.MemoryBlock;
import ghidra.program.model.symbol.Reference;
import ghidra.program.model.symbol.ReferenceIterator;

import java.io.File;
import java.io.FileWriter;
import java.io.PrintWriter;
import java.nio.charset.StandardCharsets;
import java.util.*;

public class IDMDeepAnalysisScript extends GhidraScript {

    private static final String[] TARGET_PATTERNS = {
        "video/mp4",
        "video/webm",
        "video/MP2T",
        "application/dash+xml",
        "application/vnd.apple.mpegurl",
        "mpdatoidm",
        "mpdtoidm",
        "hlstoidm",
        "hls2toidm",
        "Download this video",
        "IDM_Download_Panel_controller2_class",
        "com.tonec.idm",
        "IDMMsgHost",
        "idmbrbtn",
        "CreateIDMButton",
        "CreateIDMButton2",
        "CreateIDMButton3",
        "SetIDMButtonValue3",
        "CreateDownlSelWtIDMButton",
        "idmBroker",
        "GetNamedPipeServerProcessId",
        "GetNamedPipeClientProcessId",
        "MSG#",
        "\\\\.\\pipe\\",
        "Content-Range",
        "WinHttpOpen",
        "Referer"
    };

    @Override
    public void run() throws Exception {
        String progName = currentProgram.getName();
        println("[*] Starting IDM deep analysis on: " + progName);

        File baseOutputDir = new File("C:/Users/sayed/Downloads/IDM_RE_Investigation/ghidra");
        File reportsDir = new File(baseOutputDir, "reports");
        File decompDir = new File(baseOutputDir, "decompiled/" + progName.replace('.', '_'));
        reportsDir.mkdirs();
        decompDir.mkdirs();

        File reportFile = new File(reportsDir, progName + "_ghidra_report.md");
        PrintWriter reportWriter = new PrintWriter(new FileWriter(reportFile, StandardCharsets.UTF_8));

        reportWriter.println("# Ghidra Analysis Report: " + progName);
        reportWriter.println();
        reportWriter.println("- **Program**: `" + progName + "`");
        reportWriter.println("- **Executable Path**: `" + currentProgram.getExecutablePath() + "`");
        reportWriter.println("- **Language**: `" + currentProgram.getLanguageID() + "`");
        reportWriter.println("- **Compiler**: `" + currentProgram.getCompilerSpec().getCompilerSpecID() + "`");
        reportWriter.println("- **Image Base**: `" + currentProgram.getImageBase() + "`");
        reportWriter.println();

        // Setup decompiler
        DecompInterface decomp = new DecompInterface();
        decomp.openProgram(currentProgram);

        Set<Function> decompiledFunctions = new HashSet<>();
        Map<String, List<String>> patternMatches = new LinkedHashMap<>();

        Memory mem = currentProgram.getMemory();

        reportWriter.println("## Targeted String Occurrences and Cross-References");
        reportWriter.println();

        for (String pattern : TARGET_PATTERNS) {
            println("[*] Scanning for pattern: " + pattern);
            List<Address> matchAddrs = findStringAddresses(mem, pattern);
            if (matchAddrs.isEmpty()) {
                continue;
            }

            reportWriter.println("### Pattern: `" + pattern + "` (" + matchAddrs.size() + " occurrence(s))");
            reportWriter.println();

            List<String> details = new ArrayList<>();

            for (Address sAddr : matchAddrs) {
                reportWriter.println("- **Found at Address**: `0x" + sAddr.toString() + "`");

                // Check direct references
                Reference[] directRefs = getReferencesTo(sAddr);
                if (directRefs != null && directRefs.length > 0) {
                    reportWriter.println("  - **Direct References** (" + directRefs.length + "):");
                    for (Reference ref : directRefs) {
                        Address fromAddr = ref.getFromAddress();
                        Function func = getFunctionContaining(fromAddr);
                        String refInfo = "from `0x" + fromAddr.toString() + "` (Type: " + ref.getReferenceType() + ")";
                        if (func != null) {
                            refInfo += " in Function `[" + func.getName() + "](file:///0x" + func.getEntryPoint() + ")`";
                            decompiledFunctions.add(func);
                        } else {
                            refInfo += " (No enclosing function)";
                        }
                        reportWriter.println("    - " + refInfo);
                        details.add(refInfo);
                    }
                } else {
                    reportWriter.println("  - **Direct References**: None found.");

                    // Search for indirect references (pointers to this string address)
                    List<Address> indirectAddrs = findPointersTo(mem, sAddr);
                    if (!indirectAddrs.isEmpty()) {
                        reportWriter.println("  - **Indirect Pointer References** (" + indirectAddrs.size() + "):");
                        for (Address ptrAddr : indirectAddrs) {
                            reportWriter.println("    - Pointer at `0x" + ptrAddr.toString() + "`");
                            Reference[] ptrRefs = getReferencesTo(ptrAddr);
                            if (ptrRefs != null) {
                                for (Reference pRef : ptrRefs) {
                                    Address pFrom = pRef.getFromAddress();
                                    Function pFunc = getFunctionContaining(pFrom);
                                    String pInfo = "via ptr `0x" + ptrAddr + "` from `0x" + pFrom + "`";
                                    if (pFunc != null) {
                                        pInfo += " in Function `[" + pFunc.getName() + "](file:///0x" + pFunc.getEntryPoint() + ")`";
                                        decompiledFunctions.add(pFunc);
                                    }
                                    reportWriter.println("      - " + pInfo);
                                    details.add(pInfo);
                                }
                            }
                        }
                    } else {
                        reportWriter.println("  - **Indirect References**: None found in data sections.");
                    }
                }
            }
            reportWriter.println();
            patternMatches.put(pattern, details);
        }

        // Decompile identified functions
        reportWriter.println("## Decompiled Key Functions");
        reportWriter.println();
        reportWriter.println("Total unique functions identified for decompilation: " + decompiledFunctions.size());
        reportWriter.println();

        for (Function func : decompiledFunctions) {
            String fName = func.getName();
            Address entry = func.getEntryPoint();
            println("[*] Decompiling " + fName + " @ " + entry);

            DecompileResults results = decomp.decompileFunction(func, 60, monitor);
            String cCode = "// Decompilation failed or timed out.";
            if (results.decompileCompleted() && results.getDecompiledFunction() != null) {
                cCode = results.getDecompiledFunction().getC();
            }

            // Save individual C file
            String safeName = fName.replaceAll("[^a-zA-Z0-9_]", "_") + "_" + entry.toString();
            File cFile = new File(decompDir, safeName + ".c");
            try (PrintWriter cWriter = new PrintWriter(new FileWriter(cFile, StandardCharsets.UTF_8))) {
                cWriter.println("// Function: " + fName);
                cWriter.println("// Entry Point: 0x" + entry);
                cWriter.println("// Program: " + progName);
                cWriter.println();
                cWriter.println(cCode);
            }

            reportWriter.println("### Function `[" + fName + "](file:///" + cFile.getAbsolutePath().replace('\\', '/') + ")`");
            reportWriter.println();
            reportWriter.println("- **Entry Point**: `0x" + entry + "`");
            reportWriter.println("- **Signature**: `" + func.getSignature() + "`");

            // Callers and Callees
            Set<Function> callers = func.getCallingFunctions(monitor);
            Set<Function> callees = func.getCalledFunctions(monitor);

            reportWriter.println("- **Callers** (" + callers.size() + "): " + formatFuncList(callers));
            reportWriter.println("- **Callees** (" + callees.size() + "): " + formatFuncList(callees));
            reportWriter.println();
            reportWriter.println("```c");
            // Include first 80 lines in report
            String[] lines = cCode.split("\\r?\\n");
            int previewLines = Math.min(lines.length, 80);
            for (int i = 0; i < previewLines; i++) {
                reportWriter.println(lines[i]);
            }
            if (lines.length > previewLines) {
                reportWriter.println("// ... [Truncated in summary - Full code in " + safeName + ".c (" + lines.length + " lines)]");
            }
            reportWriter.println("```");
            reportWriter.println();
        }

        reportWriter.close();
        decomp.dispose();
        println("[+] Analysis finished. Report written to: " + reportFile.getAbsolutePath());
    }

    private List<Address> findStringAddresses(Memory mem, String text) {
        List<Address> results = new ArrayList<>();
        byte[] asciiBytes = text.getBytes(StandardCharsets.US_ASCII);
        byte[] unicodeBytes = text.getBytes(StandardCharsets.UTF_16LE);

        searchBytes(mem, asciiBytes, results);
        searchBytes(mem, unicodeBytes, results);
        return results;
    }

    private void searchBytes(Memory mem, byte[] target, List<Address> results) {
        Address addr = mem.getMinAddress();
        Address max = mem.getMaxAddress();
        while (addr != null && addr.compareTo(max) < 0) {
            addr = mem.findBytes(addr, max, target, null, true, monitor);
            if (addr != null) {
                results.add(addr);
                try {
                    addr = addr.add(target.length);
                } catch (Exception e) {
                    break;
                }
            }
        }
    }

    private List<Address> findPointersTo(Memory mem, Address targetAddr) {
        List<Address> ptrs = new ArrayList<>();
        long offset = targetAddr.getOffset();
        byte[] ptrBytes = new byte[4]; // 32-bit PE default
        ptrBytes[0] = (byte) (offset & 0xFF);
        ptrBytes[1] = (byte) ((offset >> 8) & 0xFF);
        ptrBytes[2] = (byte) ((offset >> 16) & 0xFF);
        ptrBytes[3] = (byte) ((offset >> 24) & 0xFF);

        Address addr = mem.getMinAddress();
        Address max = mem.getMaxAddress();
        while (addr != null && addr.compareTo(max) < 0) {
            addr = mem.findBytes(addr, max, ptrBytes, null, true, monitor);
            if (addr != null) {
                ptrs.add(addr);
                try {
                    addr = addr.add(4);
                } catch (Exception e) {
                    break;
                }
            }
        }
        return ptrs;
    }

    private String formatFuncList(Set<Function> funcs) {
        if (funcs == null || funcs.isEmpty()) {
            return "None";
        }
        StringBuilder sb = new StringBuilder();
        int count = 0;
        for (Function f : funcs) {
            if (count > 0) sb.append(", ");
            sb.append("`").append(f.getName()).append("` (0x").append(f.getEntryPoint()).append(")");
            count++;
            if (count >= 10) {
                sb.append("... [").append(funcs.size() - count).append(" more]");
                break;
            }
        }
        return sb.toString();
    }
}
