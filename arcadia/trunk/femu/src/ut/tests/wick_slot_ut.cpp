#include <ut/tests.h>
#include <wick/wick_slot.h>

void UT_WickSlot_Contractions_QED() {
    TParticle electron("electron", LS_NORMAL);
    TParticle photon("photon", LS_WAVE);

    TWickSlot inputElectron1(1, 1, false, true);
    inputElectron1.InitializeFreedomDegree(&electron, 1);
    inputElectron1.InitializeFreedomDegree(&photon, 0);

    TWickSlot inputElectron2(2, 1, false, true);
    inputElectron2.InitializeFreedomDegree(&electron, 1);
    inputElectron2.InitializeFreedomDegree(&photon, 0);
    
    TWickSlot ouputElectron1(3, 2, false, true);
    ouputElectron1.InitializeFreedomDegree(&electron, 1);
    ouputElectron1.InitializeFreedomDegree(&photon, 0);
    
    TWickSlot ouputElectron2(4, 2, false, true);
    ouputElectron2.InitializeFreedomDegree(&electron, 1);
    ouputElectron2.InitializeFreedomDegree(&photon, 0);
    
    TWickSlot QEDVertex1(5, 0, false, false);
    QEDVertex1.InitializeFreedomDegree(&electron, 2);
    QEDVertex1.InitializeFreedomDegree(&photon, 1);
    
    TWickSlot QEDVertex2(5, 0, false, false);
    QEDVertex2.InitializeFreedomDegree(&electron, 2);
    QEDVertex2.InitializeFreedomDegree(&photon, 1);

    VERIFY(!TWickSlot::EquivalentSlots(inputElectron1, inputElectron2));
    VERIFY(!TWickSlot::EquivalentSlots(ouputElectron1, ouputElectron2));
    VERIFY(!TWickSlot::EquivalentSlots(inputElectron1, ouputElectron1));
    VERIFY(!TWickSlot::EquivalentSlots(inputElectron1, ouputElectron2));
    VERIFY(!TWickSlot::EquivalentSlots(inputElectron1, QEDVertex1));
    VERIFY(TWickSlot::EquivalentSlots(QEDVertex1, QEDVertex2));

    VERIFY(!TWickSlot::AllowContraction(inputElectron1, inputElectron2, &electron));
    VERIFY(!TWickSlot::AllowContraction(ouputElectron1, ouputElectron2, &electron));
    
    VERIFY(TWickSlot::AllowContraction(inputElectron1, ouputElectron1, &electron));
    VERIFY(TWickSlot::AllowContraction(inputElectron1, ouputElectron2, &electron));

    VERIFY(TWickSlot::AllowContraction(QEDVertex1, QEDVertex2, &photon));
    VERIFY(TWickSlot::AllowContraction(inputElectron1, QEDVertex1, &electron));
    VERIFY(!TWickSlot::AllowContraction(inputElectron1, QEDVertex1, &photon));

    VERIFY(!TWickSlot::AllowContraction(QEDVertex1, QEDVertex1, &electron));

    VERIFY(!inputElectron1.IsFinalized());
    VERIFY(!QEDVertex1.IsFinalized());

    VERIFY(TWickSlot::AllowContraction(QEDVertex1, QEDVertex2, &photon));
    TWickSlot::Contract(QEDVertex1, QEDVertex2, &photon);

    VERIFY(TWickSlot::AllowContraction(inputElectron1, QEDVertex1, &electron));
    TWickSlot::Contract(inputElectron1, QEDVertex1, &electron);

    VERIFY(TWickSlot::AllowContraction(inputElectron2, QEDVertex1, &electron));
    TWickSlot::Contract(inputElectron2, QEDVertex1, &electron);

    VERIFY(TWickSlot::AllowContraction(ouputElectron1, QEDVertex2, &electron));
    TWickSlot::Contract(ouputElectron1, QEDVertex2, &electron);

    VERIFY(ouputElectron2.GetPendingParticleType() == &electron);
    VERIFY(QEDVertex1.IsFinalized());
    VERIFY(QEDVertex2.GetPendingParticleType() == &electron);

    VERIFY(TWickSlot::AllowContraction(ouputElectron2, QEDVertex2, &electron));
    TWickSlot::Contract(ouputElectron2, QEDVertex2, &electron);

    VERIFY(QEDVertex2.IsFinalized());
    VERIFY(inputElectron1.IsFinalized());

    VERIFY(!TWickSlot::AllowContraction(QEDVertex1, QEDVertex2, &photon));
    TWickSlot::BreakContraction(QEDVertex1, QEDVertex2, &photon);
    VERIFY(TWickSlot::AllowContraction(QEDVertex1, QEDVertex2, &photon));

    VERIFY(QEDVertex1.GetPendingParticleType() == &photon);
    VERIFY(QEDVertex2.GetPendingParticleType() == &photon);
}

void UT_WickSlot_Contractions_F4() {
    TParticle scalarBoson("scalar boson", LS_DASHED);

    TWickSlot inputBoson(1, 1, true, true);
    inputBoson.InitializeFreedomDegree(&scalarBoson, 1);

    TWickSlot ouputBoson(2, 2, true, true);
    ouputBoson.InitializeFreedomDegree(&scalarBoson, 1);

    TWickSlot F4Vertex(3, 0, true, false);
    F4Vertex.InitializeFreedomDegree(&scalarBoson, 4);

    VERIFY(TWickSlot::AllowContraction(inputBoson, F4Vertex, &scalarBoson));
    TWickSlot::Contract(inputBoson, F4Vertex, &scalarBoson);

    VERIFY(TWickSlot::AllowContraction(F4Vertex, F4Vertex, &scalarBoson));
    TWickSlot::Contract(F4Vertex, F4Vertex, &scalarBoson);

    VERIFY(TWickSlot::AllowContraction(F4Vertex, ouputBoson, &scalarBoson));
    TWickSlot::Contract(F4Vertex, ouputBoson, &scalarBoson);

    VERIFY(!TWickSlot::AllowContraction(F4Vertex, F4Vertex, &scalarBoson));
    TWickSlot::BreakContraction(F4Vertex, F4Vertex, &scalarBoson);
    VERIFY(TWickSlot::AllowContraction(F4Vertex, F4Vertex, &scalarBoson));
}

void UT_WickSlot() {
    MESSAGE("WickSlot unittest...");
    UT_WickSlot_Contractions_QED();
    UT_WickSlot_Contractions_F4();
    MESSAGE("WickSlot unittest: ok");
}
