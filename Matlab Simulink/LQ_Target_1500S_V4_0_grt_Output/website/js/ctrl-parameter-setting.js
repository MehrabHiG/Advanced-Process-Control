moduleSWV.controller("ctrl-parameter-setting", function ($scope, $log, $http) {
    $log.log("[ctrl-parameter-setting]: wurde initialisiert");
    $scope.ListTyp = "";
    $scope.Parameter = "";
    $scope.tmpFile = "";
    $scope.writeStatus = "";
    $scope.ReloadFilter = "";
    $scope.EnableFilter = "";

    $scope.$on("open-params", function (event, data, access,enableFilter) {
        $scope.ListTyp = data;
        $log.log("[ctrl-parameter-setting]: show " + data);

        $scope.EnableFilter = enableFilter;

        if (data === "WorkspaceParams") {
            $scope.tmpFile = 'param/'+$scope.ModelName+'_WorkspaceParameter.json';
            $scope.readParams("noFilter");
        }

        if (data === "ModelWorkspaceParams") {
            $scope.tmpFile = 'param/' + $scope.ModelName + '_ModelWorkspaceParameter.json';
            $scope.readParams("noFilter");
        }

        if (data === "Params") {
            $scope.tmpFile = 'param/' + $scope.ModelName + '_Parameter.json';
            //Filter Block Parameter
            if (access === "all") {
                $scope.readParams("noFilter");
            } else {
                $scope.readParams(access);
            }


        }

    });

    $scope.closeParams = function () {
        $scope.Parameter = "";
        $scope.$emit("close-Params")
    }

    $scope.writeParams = function (filter) {
        $log.log("[ctrl-parameter-setting]: Write " + $scope.Parameter[this.$index].name + " to the plc .")
        $log.log("[ctrl-parameter-setting]: DB " + $scope.Parameter[this.$index].plcPath);

        $scope.writeStatus = "write to plc ...";
		
		$http.post($scope.tmpFile, '"gDB_SWV_Interface".Write=1');
		var val = $scope.valueReplacer($scope.Parameter[this.$index].value);
        $http.post($scope.tmpFile, $scope.Parameter[this.$index].plcPath + "=" + val)
            .then(function (response) {
				$http.post($scope.tmpFile, '"gDB_SWV_Interface".Write=2');
                $scope.writeStatus = "write to plc finished";
                if($scope.EnableFilter){
                    $scope.readParams("Reload");
                }else{
                    $scope.readParams("noFilter");
                }
            }), function (response) {
            $log.log("[ctrl-parameter-setting]: Error while writing Parameters");
        };
    }

    $scope.readParams = function (filter) {
        $http.get($scope.tmpFile,{headers:{'If-Modified-Since': '0',
    "Pragma": "no-cache",
    "Expires": -1,
    "Cache-Control": "no-cache, no-store, must-revalidate"}})
            .then(function (response) {
                $scope.Parameter = $scope.replacer(response.data.Parameters);
                $scope.ParameterTemp = $scope.Parameter;
                if($scope.EnableFilter){
                    $log.log("Read parameters from plc with filter for blocks");
                    if(filter==="Reload"){
                        for (let index = 0; index < $scope.Parameter.length; index++) {
                            if ($scope.Parameter[index].BlockID != $scope.ReloadFilter) {
                                $scope.Parameter.splice(index, 1);
                                index--;
                            } else {
                                $log.log($scope.Parameter[index].BlockID);
                            }
                        }
                    }else{
                        for (let index = 0; index < $scope.Parameter.length; index++) {
                            if ($scope.Parameter[index].BlockID != filter) {
                                $scope.Parameter.splice(index, 1);
                                index--;
                            } else {
                                $log.log($scope.Parameter[index].BlockID);
                            }
                        }
                        $scope.ReloadFilter = filter;
                    }
                    //BugFix
                    if ($scope.Parameter.length === 0) {
                        $scope.closeParams();
                    }
                }else{
                    $log.log("Read parameters from plc");
                }
                $scope.writeStatus = "";
            }, function (response) {
                $log.log("[ctrl-parameter-setting]: error while reading Parameters");
            });
    }

});