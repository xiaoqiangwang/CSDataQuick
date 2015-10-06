ASG(DEFAULT) {
    RULE(1, READ)
    RULE(1, WRITE)
}

ASG(limit) {
    INPA(writable)
    RULE(1, READ)
    RULE(1, WRITE) {
        CALC("A=1")
    }
}
