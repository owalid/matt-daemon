#ifndef EVENT_ENUM_HPP
# define EVENT_ENUM_HPP

enum EventEnum{
    programStart,
    serverCreated,
    daemonModeStarted,
    serverStartedPid,
    signalHandler,
    programQuit,
    errorFileLock,
    errorFileUnlock,
    errorNonRootUser,
    userConnection,
    userInput,
    userRequest,
    userRequestRejected,
    archiveCreated,
    errorArchive,
    connectionClosed,
    procConnectorStarted,
    procConnectorStopped,
    procConnectorDisconnected,
    procEventFork,
    procEventExec,
    procEventExit,
    procEventUid,
    procEventGid,
    procEventSid,
    procEventPtrace,
    procEventComm,
    procEventCoreDump,
    procEventUnknown,
    procEventNone
};

#endif
