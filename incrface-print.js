Module = {
    print: (...args) => {
        const text = args.join(' ');
        // console.log('incrface received');
        // console.log(text);
        if(window.receiveIncr)
            window.receiveIncr(text)
    }
};
