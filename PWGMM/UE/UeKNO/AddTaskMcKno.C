///////////////////////////////////////////////////////////////////
//                                                               //
//            AddTaskMcKno Macro to run on grids               //
//                                                               //
//                                                               //
///////////////////////////////////////////////////////////////////
class AliAnalysisDataContainer;

AliAnalysisTaskMcKno* AddTaskMcKno(const Char_t* taskname="McKno", Bool_t  useMC  = kTRUE, Bool_t performMCclosuretest = kFALSE, Double_t minpT=0.5)
{
    // get the manager via the static access member. since it's static, you don't need
    // an instance of the class to call the function


    AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
    if (!mgr) {
        return 0x0;
    }
    // get the input event handler this handler is part of the managing system and feeds events to your task
    if (!mgr->GetInputEventHandler()) {
        return 0x0;
    }

    // now you create an instance of your task
    AliAnalysisTaskMcKno* taskKno = new AliAnalysisTaskMcKno("taskKno");
    if(!taskKno) return 0x0;
    taskKno->SetUseMC(useMC);
    taskKno->SetMCclosureTest(performMCclosuretest);
    // add your task to the manager
    taskKno->SetPtMin(minpT);
    mgr->AddTask(taskKno);


    mgr->ConnectInput(taskKno,0,mgr->GetCommonInputContainer());
    mgr->ConnectOutput(taskKno,1,mgr->CreateContainer(Form("cList%s_%1.2f",taskname,minpT), TList::Class(), AliAnalysisManager::kOutputContainer, Form("%s:%s", AliAnalysisManager::GetCommonFileName(),taskname)));

    return taskKno;
}