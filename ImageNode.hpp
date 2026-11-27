#ifndef IMAGENODE_HPP
#define IMAGENODE_HPP
#include <QSGSimpleTextureNode>
#include <QQuickItem>
#include <QSGTexture>
#include <QQuickWindow>
#include<QRect>

class Q_ImageNode : public QSGSimpleTextureNode{
public:
    void setTextureFromImage(const QImage& img, QQuickItem* contextItem){
        if(contextItem != nullptr){
            // qDebug() <<"proceeding!";
            QSGTexture* tex = contextItem->window()->createTextureFromImage(img, QQuickWindow::CreateTextureOption::TextureHasAlphaChannel);
            this->setTexture(tex);
        }
    }
    Q_ImageNode(QQuickItem* contextItem =nullptr, QSGTexture* tex =nullptr, const QQuickWindow::CreateTextureOption option = QQuickWindow::TextureHasAlphaChannel, const bool ownsTexture = false, QRect override_rect = QRect(-1, -1, -1, -1) ) {
        this ->setOwnsTexture(ownsTexture);
        if( override_rect == QRect(-1,-1,-1,-1) ) {
            this ->setRect( contextItem->boundingRect().toRect() ); }
        else
            this->setRect(override_rect);
        this->setTexture(tex);
    }
    Q_ImageNode(const QImage& img, QQuickItem* contextItem =nullptr,const QQuickWindow::CreateTextureOption option = QQuickWindow::TextureHasAlphaChannel, const bool ownsTexture = false, const QRect override_rect = QRect(-1, -1, -1, -1) ){
        this ->setOwnsTexture(ownsTexture);
        if( override_rect == QRect(-1,-1,-1,-1) ) {
            this ->setRect( contextItem->boundingRect().toRect() ); }
        else
            this->setRect(override_rect);
        this->setTextureFromImage(img, contextItem);
    }
private:
};

#endif // IMAGENODE_HPP