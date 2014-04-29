#include <ut/tests.h>
#include "wick_example.h"
#include <wick/wick_slot.h>
#include <wick/wick_task.h>
#include <feyn/diagram.h>

void GenerateContractionsForSampleTask() {
    MESSAGE("Generating an instance of TWickTask...");

    TParticle scalarBoson("scalar boson", LS_DASHED);

    TWickSlot inputBoson1(1, 1, false, true);
    inputBoson1.InitializeFreedomDegree(&scalarBoson, 1);

    TWickSlot inputBoson2(1, 1, false, true);
    inputBoson2.InitializeFreedomDegree(&scalarBoson, 1);

    TWickSlot ouputBoson1(2, 2, false, true);
    ouputBoson1.InitializeFreedomDegree(&scalarBoson, 1);
    
    TWickSlot ouputBoson2(2, 2, false, true);
    ouputBoson2.InitializeFreedomDegree(&scalarBoson, 1);

    TWickSlot F4Vertex1(3, 0, true, false);
    F4Vertex1.InitializeFreedomDegree(&scalarBoson, 4);
    
    // Uncomment this if you want another interaction vertex
    // TWickSlot F4Vertex2(1, 4, 0, true, false);
    // F4Vertex2.InitializeFreedomDegree(&scalarBoson, 4);

    TWickTask task;
    task.Slots << inputBoson1 << inputBoson2 << ouputBoson1 << ouputBoson2;
    task.Slots << F4Vertex1;

    // And this
    // task.Slots << F4Vertex2;
    
    MESSAGE("Solving...");

    QVector<TDiagram*> res;
    task.Solve(&res, false);

    MESSAGE("Solved (" << res.size() << " diagrams). Writing DOT to files");

    for (int i = 0; i < res.size(); i++) {
        QFile file("junk/d" + QString::number(i + 1) + ".dot");
        file.open(QIODevice::WriteOnly);
        
        QTextStream stream(&file);
        stream << res[i]->ExportToDot("Diagram_" + QString::number(i + 1));
        stream.flush();
    }
}
