#include "compass_panel.h"

CompassPanel::CompassPanel(QWidget *parent) : QWidget(parent)
{
    int c;
    QPalette palette0;
    for ( c = 0; c < QPalette::NColorRoles; c++ )
    {
        const QPalette::ColorRole colorRole =
            static_cast<QPalette::ColorRole>( c );

        palette0.setColor( colorRole, QColor() );
    }

    palette0.setColor( QPalette::Base,
        palette().color( backgroundRole() ).light( 120 ) );
    palette0.setColor( QPalette::WindowText,
        palette0.color( QPalette::Base ) );

    compass = new QwtCompass( this );
    compass->setLineWidth( 4 );
    compass->setFrameShadow(QwtCompass::Raised );

    /*
     A compass with a yellow on black ray
     */
    palette0.setColor( QPalette::WindowText, Qt::black );

    compass->setNeedle( new QwtDialSimpleNeedle( QwtDialSimpleNeedle::Ray,
        false, Qt::yellow ) );
    compass->setValue( 315.0 );

    QPalette newPalette = compass->palette();
    for ( c = 0; c < QPalette::NColorRoles; c++ )
    {
        const QPalette::ColorRole colorRole =
            static_cast<QPalette::ColorRole>( c );

        if ( palette0.color( colorRole ).isValid() )
            newPalette.setColor( colorRole, palette0.color( colorRole ) );
    }

    for ( int i = 0; i < QPalette::NColorGroups; i++ )
    {
        const QPalette::ColorGroup colorGroup =
            static_cast<QPalette::ColorGroup>( i );

        const QColor light =
            newPalette.color( colorGroup, QPalette::Base ).light( 170 );
        const QColor dark = newPalette.color( colorGroup, QPalette::Base ).dark( 170 );
        const QColor mid = compass->frameShadow() == QwtDial::Raised
            ? newPalette.color( colorGroup, QPalette::Base ).dark( 110 )
            : newPalette.color( colorGroup, QPalette::Base ).light( 110 );

        newPalette.setColor( colorGroup, QPalette::Dark, dark );
        newPalette.setColor( colorGroup, QPalette::Mid, mid );
        newPalette.setColor( colorGroup, QPalette::Light, light );
    }

    compass->setPalette( newPalette );
    compass->setEnabled(false);

//    compass->setFixedHeight(250);
    compass->setFixedSize(250,250);

    table = new QTableWidget;

    QHBoxLayout *layout_h = new QHBoxLayout;
    layout_h->addWidget(compass);
    layout_h->addWidget(table);
    setLayout(layout_h);

//    setFixedHeight(250);
}
