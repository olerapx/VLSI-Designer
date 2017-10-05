#include "statistics.h"

Statistics::Statistics(int topLevel) :
    topLevel(topLevel)
{

}

void Statistics::merge(Statistics& other)
{
    if(other.getTopLevel() < topLevel)
        throw IllegalArgumentException(QObject::tr("Cannot merge statistics from level %1 to level %2.")
                                       .arg(QString::number(other.getTopLevel()), QString::number(topLevel)));
    // TODO
}
