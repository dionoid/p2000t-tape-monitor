#include "blockmap.h"

BlockMap::BlockMap(unsigned int _columns,
                   unsigned int _rows,
                   unsigned int _blocksize,
                   QWidget *parent) : QWidget(parent) {

    // set dimensions of blockmap
    this->columns = _columns;
    this->rows = _rows;
    this->blocksize = _blocksize;
    this->width = this->columns * this->blocksize + 1;
    this->height = this->rows * this->blocksize + 1;

    // contains values to plot
    this->blockvalues = std::vector<uint8_t>(this->rows * this->columns, 0x00);

    this->setBackgroundRole(QPalette::Base);
    this->setAutoFillBackground(false);
    this->setUpdatesEnabled(true);
}

/**
 * @brief Custom function wherein active blocks are highlighted
 * @param _blocks
 */
void BlockMap::set_blocklist(const std::vector<std::pair<uint8_t, uint8_t>>& _blocks) {
    memset(this->blockvalues.data(), 0x00, this->blockvalues.size());
    for(const auto& block : _blocks) {
        this->blockvalues[block.first * this->columns + block.second] = 0x01;
    }
    this->update();
}

void BlockMap::set_cache(const std::vector<uint8_t> _cache_status) {
    if(this->blockvalues.size() != _cache_status.size()) {
        qCritical() << "Blockvalue size " << this->blockvalues.size() << " does not match " << _cache_status.size();
        throw std::runtime_error("Invalid sync update received.");
    }

    this->blockvalues = _cache_status;
    this->update();
}

QSize BlockMap::minimumSizeHint() const {
    return QSize(this->width, this->height);
}

QSize BlockMap::sizeHint() const {
    return QSize(this->width, this->height);
}

void BlockMap::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setPen(QPen(Qt::black));


    for(unsigned int j=0; j<this->rows; j++) {
        for(unsigned int i=0; i<this->columns; i++) {
            QRect rect(i * this->blocksize,
                       j * this->blocksize,
                       this->blocksize,
                       this->blocksize);

            switch(this->blockvalues[j * this->columns + i]) {
                case 0x00:
                    painter.setBrush(QBrush(Qt::gray));
                break;
                case 0x01:
                    painter.setBrush(QBrush(Qt::green));
                break;
                case 0x02:
                    painter.setBrush(QBrush(QColor("orange")));
                break;
                case 0x03:
                    painter.setBrush(QBrush(Qt::red));
                break;
                default:
                    painter.setBrush(QBrush(Qt::gray));
                break;
            }
            painter.drawRect(rect);
        }
    }
}
