var moduleSWV =  angular.module("SWV",  ['ngSanitize'])
    .run(function ( $log,$http) {
        $log.log("[SWV]: App started.");
    })
    .directive('stringToNumber', function() {
            return {
                require: 'ngModel',
                link: function(scope, element, attrs, ngModel) {
                    ngModel.$parsers.push(function(value) {
                        return '' + value;
                    });
                    ngModel.$formatters.push(function(value) {
                        return parseFloat(value);
                    });
                }
            };
    });