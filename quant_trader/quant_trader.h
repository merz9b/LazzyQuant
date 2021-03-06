#ifndef QUANT_TRADER_H
#define QUANT_TRADER_H

#include <functional>
#include <boost/optional.hpp>
#include <QObject>
#include <QMap>

class Bar;
class BarCollector;
class AbstractIndicator;
class AbstractStrategy;

class QuantTrader : public QObject
{
    Q_OBJECT

protected:
    // Following QString keys are instumentIDs
    QMap<QString, BarCollector*> collector_map;
    QMap<QString, QMap<int, QList<Bar>>> bars_map;
    QMap<QString, QMultiMap<int, AbstractIndicator*>> indicatorMap;
    QMultiMap<QString, AbstractStrategy*> strategy_map;
    QMap<QString, boost::optional<int>> position_map;

    QString kt_export_dir;
    bool saveBarsToDB;
    QString dbDriver;
    QString dbHostName;
    QString dbUserName;
    QString dbPassword;

    void loadQuantTraderSettings();
    void loadTradeStrategySettings();
    QList<Bar>* getBars(const QString &instrumentID, int timeFrame);

    QString currentInstrumentID;
    int currentTimeFrame;

public:
    explicit QuantTrader(bool saveBarsToDB, QObject *parent = 0);
    ~QuantTrader();

    std::function<void(const QString&, int)> setPosition = [](auto, auto) -> void {};
    std::function<void(const QString&)> cancelAllOrders = [](auto) -> void {};

    AbstractIndicator* registerIndicator(const QString &instrumentID, int timeFrame, QString indicator_name, ...);

private slots:
    void onNewBar(const QString &instrumentID, int timeFrame, const Bar &bar);

public slots:
    void setTradingDay(const QString &tradingDay);
    void onMarketData(const QString &instrumentID, int time, double lastPrice, int volume,
                      double askPrice1, int askVolume1, double bidPrice1, int bidVolume1);
    void onMarketPause();
    void onMarketClose();
    bool checkDataBaseStatus();
    void quit();
};

#endif // QUANT_TRADER_H
