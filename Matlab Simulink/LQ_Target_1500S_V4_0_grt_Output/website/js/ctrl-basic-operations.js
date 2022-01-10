/*      controller for basic operations of simulink web visu
 *
 *      This controller provide some basic operations of the simulink web visualization.
 *      Additionally to the operations provide this controller the main application struct
 *      of the single-page-application.
 */

moduleSWV.controller("ctrl-basic-operations", function ($scope, $log, $http, $sce, $interval) {
    $log.log("[ctrl-basic-operations]: wurde initialisiert");

    $scope.TPS_Values = 0;
    $scope.Chart_Values = 0;
    //Settings
    $scope.ModelName = "";
    $scope.ShowMenueP = false;
    $scope.ShowMenueWSP = false;
    $scope.ShowMenueMWSP = false;
    $scope.ShowParams = false;
    $scope.ShowTrace = false;
    $scope.ShowBackSvgFunction = false;
    $scope.Configuration = "";
    $scope.CurrentSvgModel = "";
    $scope.PreSvgModels = [];
    $scope.CurrentTrace = "";
    $scope.SubSystemName = "";
	$scope.oldState = "-1";
	$scope.oldChart = "-1";
	

    $http.get("param/config.json",{headers:{'If-Modified-Since': '0',
    "Pragma": "no-cache",
    "Expires": -1,
    "Cache-Control": "no-cache, no-store, must-revalidate"}}).then(function (response) {
        $scope.Configuration = response.data;
        $scope.ModelName = $scope.Configuration.ModelName;
        $scope.ShowMenueP = $scope.Configuration.Parameter;
        $scope.ShowMenueWSP = $scope.Configuration.WorkspaceParameter;
        $scope.ShowMenueMWSP = $scope.Configuration.ModelWorkspaceParameter;
        $scope.SubSystemName = $scope.Configuration.Title.replace('<',/&lt;/g).replace('>',/&gt;/g);
    });

    $http.get("param/Model.json",{headers:{'If-Modified-Since': '0',
    "Pragma": "no-cache",
    "Expires": -1,
    "Cache-Control": "no-cache, no-store, must-revalidate"}}).then(function (response) {
        $scope.openSvg($scope.replacer(response.data));
    });

    //Function - Area
    $scope.trustedHtml = function (plainText) {
        return $sce.trustAsHtml(plainText);
    }

    $scope.openPreviousSvg = function () {
		if ($scope.PreSvgModels.length > 0) {
			$scope.openSvg($scope.PreSvgModels[$scope.PreSvgModels.length - 1], true);
        }
    };

    $scope.openSvg = function (targetModel, isPreviousSvg) {
        if ($scope.areTheSameSvg($scope.CurrentSvgModel,targetModel) === false) {
			// clear Chart marks
			if($scope.oldState !== "-1"){
				var m_svg = document.getElementById("SVG_Graphic");
				var m_svgDoc = svg.contentDocument;
				if (svgDoc !== null && typeof ($scope.Chart_Values.StflwChartStates) !== 'undefined') {
					m_StFlwSate = svgDoc.getElementById($scope.oldState);
					if(m_StFlwSate!==null){
						m_StFlwSate = m_StFlwSate.childNodes[1];
						m_StFlwSate.setAttribute("style","fill: black; font-weight: 200;");
						$scope.oldState = "-1";
					}
				}
			}
			var fullPathToSvg = targetModel.FullBlockPath.replace(new RegExp(" ", 'g'),"");
            $log.log("[ctrl-basic-operations]: call svg model:" + fullPathToSvg);
            $scope.$broadcast("open-SVG", fullPathToSvg);
			if (($scope.CurrentSvgModel !== "") && ((typeof(isPreviousSvg) === "undefined") || (isPreviousSvg === false))) {
				$scope.PreSvgModels[$scope.PreSvgModels.length] = $scope.CurrentSvgModel;
			}
            $scope.CurrentSvgModel = targetModel;
            $scope.Configuration.Title = targetModel.Name;
            $scope.SubSystemName = targetModel.Name.replace('<',/&lt;/g).replace('>',/&gt;/g);
			if ((typeof(isPreviousSvg) !== "undefined") && (isPreviousSvg === true)) {
				$scope.PreSvgModels.pop();
			}
        }
		if ($scope.PreSvgModels.length === 0) {
			$scope.ShowBackSvgFunction = false;
		}
		else {
			$scope.ShowBackSvgFunction = true;
		}
        $scope.$applyAsync();
    };
	
	$scope.areTheSameSvg = function (model1, model2) {
		if ( (typeof(model1.FullBlockPath) !== "undefined") && (typeof(model2.FullBlockPath) !== "undefined") ) {
			var path1 = model1.FullBlockPath.replace(new RegExp(" ", 'g'),"");
			var path2 = model2.FullBlockPath.replace(new RegExp(" ", 'g'),"");
			if (path1 === path2) {
				return true;
			}
		}
		return false;
	}

    $scope.openParams = function (typ, access, enableFilter) {
        $log.log("[ctrl-basic-operations]: show param settings");
        $scope.ShowParams = true;
        $scope.$broadcast("open-params", typ, access, enableFilter);
    };

    //Data Exchange between cpu and browser for Internal Signals
    $scope.readTPS = function () {
        $http.get("param/TestPointedSignals.json",{headers:{'If-Modified-Since': '0',
    "Pragma": "no-cache",
    "Expires": -1,
    "Cache-Control": "no-cache, no-store, must-revalidate"}}).then(function (response) {
            $scope.TPS_Values = $scope.replacer(response.data);
			$scope.$broadcast("edit-TPS");
        });
    }
	
	//Data Exchange between cpu and browser for StateFlowCharts
	 $scope.readCHARTS = function () {
        $http.get("param/ChartSignals.json",{headers:{'If-Modified-Since': '0',
    "Pragma": "no-cache",
    "Expires": -1,
    "Cache-Control": "no-cache, no-store, must-revalidate"}}).then(function (response) {
            $scope.Chart_Values = $scope.replacer(response.data);
			$scope.$broadcast("edit-CHARTS");
        });
    }
    $interval(function () {
        $scope.readTPS();
		$scope.readCHARTS();
    }, NaN);

    $scope.$on("close-Params", function (event) {
        $scope.ShowParams = false;
    })

    $scope.$on("close-Trace", function (event) {
        $scope.ShowTrace = false;
    })

    $scope.openTrace = function (name_of_config) {
        $scope.ShowTrace = true;
        $scope.CurrentTrace = name_of_config;
		let tracescope = angular.element($("#SimulinkScope")).scope();
		tracescope.setParamsForCurrentTrace();
    }

    $scope.replacer = function (input) {
        var test = JSON.stringify(input);
        test = test.replace(new RegExp("(\&#x2d;)", 'g'), "-");
		test = test.replace(new RegExp("(\&#x2b;)", 'g'), "+");
		test = test.replace(new RegExp("(\&#x24;7FF0000000000000)", 'g'), "Inf");
		test = test.replace(new RegExp("(\&#x24;FFF0000000000000)", 'g'), "-Inf");
		test = test.replace(new RegExp("(\&#x24;FFF8000000000000)", 'g'), "NaN");
        return JSON.parse(test);
    }

    $scope.valueReplacer = function (input) {
		var inp = input.toString().toLowerCase();
		if (inp.indexOf("e+") > -1) {
			return inp.replace("e+", "e");
		}
  		if (inp === "+inf" || inp === "inf") {
        	return "1.7976931348623157E308"; // workaround
        }
  		else if (inp === "-inf") {
        	return "-1.7976931348623157E308"; // workaround
        }
  		else if (inp === "nan") {
        	// TODO: NaN is not handled
        }
		return input;
    }
});