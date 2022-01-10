function WrapperOpenSvg(Subsystem) {
    let scope = angular.element($("#Page")).scope();
    scope.openSvg(Subsystem);
}

function WrapperOpenTrace(nameOfConfig) {
    let scope = angular.element($("#Page")).scope();
    scope.openTrace(nameOfConfig);
}

