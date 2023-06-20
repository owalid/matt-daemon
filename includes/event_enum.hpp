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
    archiveCreated,
    errorArchive,
    connectionClosed,
    procConnectorStarted,
    procConnectorDisconnected,
    procEventFork,
    procEventExec,
    procEventExit,
    procEventUid,
    procEventGid,
    procEventNone
};

#endif
