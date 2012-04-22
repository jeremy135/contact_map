//Drupal.behaviors.contact_map = function (context) {
jQuery(document).ready( function() {
    jQuery('#plus a').click(function(e) {
        jQuery('#myMap').attr('style', cm.maxSize);
        jQuery('#plus').addClass('hide');
        jQuery('#minus').removeClass('hide');
        e.preventDefault();
    })
    jQuery('#minus a').click(function(e) {
        jQuery('#myMap').attr('style', cm.minSize);
        jQuery('#minus').addClass('hide');
        jQuery('#plus').removeClass('hide');
        e.preventDefault();
    })
})
//}